#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

#define MAX_LINE_LENGTH 256          // размер рабочих строковых буферов
#define MAX_DEVICE_TITLE_LENGTH 128 

// структура элемента результата от драйвера (должна совпадать с layout в драйвере)
typedef struct _PCI_DEVICE_INFO {
    UINT16 vendorID;  // 16-битный идентификатор производителя
    UINT16 deviceID;  // 16-битный идентификатор устройства
} PCI_DEVICE_INFO;

// код IOCTL для запроса сканирования PCI 
#define IOCTL_SCAN_PCI CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
 
static void printSeparator(int width)
{
    for (int i = 0; i < width; ++i) putchar('-');
    putchar('\n');
}

/*
 * поиск человекочитаемого названия по паре (vendorID, deviceID) в pci.ids
 * формат pci.ids
 */
static const char* getDeviceInfo(PCI_DEVICE_INFO dev, FILE* pciFile)
{
    static char result[MAX_LINE_LENGTH];   // “[Vendor] Device” или “[Unknown …] …”
    char line[MAX_LINE_LENGTH];            // текущая строка файла
    char vendorString[5], deviceString[5]; // целевые vvvv/dddd (с нулём)

    // подготовить ключи поиска в нижнем регистре
    snprintf(vendorString, sizeof(vendorString), "%04x", dev.vendorID);
    snprintf(deviceString, sizeof(deviceString), "%04x", dev.deviceID);

    int foundVendor = 0;                   // флаг: строка производителя найдена
    memset(result, 0, sizeof(result));
    fseek(pciFile, 0, SEEK_SET);          // начало файла

    // построчно прочитать pci.ids
    while (fgets(line, sizeof(line), pciFile)) {
        // пропустить комментарии и пустые строки
        if (line[0] == '#' || line[0] == '\n') continue;

        // шаг 1: распознать производителя: "vvvv<space>..."
        if (!foundVendor && strncmp(line, vendorString, 4) == 0 && line[4] == ' ') {
            foundVendor = 1;

            // извлечь текст производителя
            char vendorName[MAX_LINE_LENGTH];
#if defined(_MSC_VER)
            sscanf_s(line, "%*s %[^\n]", vendorName, (unsigned)MAX_LINE_LENGTH);
#else
            sscanf(line, "%*s %[^\n]", vendorName);
#endif
            // записать в result: "[Vendor]"
            snprintf(result, sizeof(result), "[%s]", vendorName);
        }
        // шаг 2: после производителя распознать устройство: "\tdddd<space>..."
        else if (foundVendor && line[0] == '\t' && strncmp(&line[1], deviceString, 4) == 0 && line[5] == ' ') {
            char deviceDesc[MAX_LINE_LENGTH];
#if defined(_MSC_VER)
            sscanf_s(line, "\t%*s %[^\n]", deviceDesc, (unsigned)MAX_LINE_LENGTH);
#else
            sscanf(line, "\t%*s %[^\n]", deviceDesc);
#endif
            // дополнить result: "[Vendor] Device"
            snprintf(result + strlen(result), sizeof(result) - strlen(result), " %s", deviceDesc);
            break;
        }
        // шаг 3: производитель найден, но нет строки устройства — вернуть Unknown Device
        else if (foundVendor && line[0] != '\t') {
            snprintf(result + strlen(result), sizeof(result) - strlen(result), " Unknown Device");
            break;
        }
    }

    // обработка случая: производитель не найден
    if (!foundVendor) {
        return "[Unknown Vendor] Unknown Device";
    }
    return result;
}

/*
 * поток выполнения:
 *  1) настроить локаль/кодовые страницы для кириллицы
 *  2) открыть символьное устройство \\.\PciScanner
 *  3) отправить IOCTL_SCAN_PCI, получить массив PCI_DEVICE_INFO
 *  4) открыть pci.ids (argv[1] либо .\pci.ids)
 *  5) вывести таблицу: “[Vendor] Device | VendorID | DeviceID”
 */
int wmain(int argc, wchar_t** argv)
{
    (void)argc; (void)argv;

    // кодовые страницы консоли: ввод/вывод в CP1251
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    // путь к pci.ids по умолчанию: рядом с exe
    const wchar_t* defaultPciIds = L".\\pci.ids";

    // открытие символьного устройства драйвера 
    HANDLE hDevice = CreateFileW(L"\\\\.\\PciScanner",
        GENERIC_READ | GENERIC_WRITE,  // чтение/запись: METHOD_BUFFERED
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        // типовой сбой: при запуске без админа
        printf("Ошибка при открытии \\\\.\\PciScanner (код %lu)\n", GetLastError());
        printf("Нажмите Enter для завершения...\n");
        (void)_getch();
        return 2;
    }

    // заготовить буфер под ответ драйвера (до 1024 записей)
    enum { MAX_DEVS = 1024 };
    PCI_DEVICE_INFO devices[MAX_DEVS];
    DWORD bytesReturned = 0;

    // запросить сканирование PCI у драйвера
    BOOL ok = DeviceIoControl(hDevice,
        IOCTL_SCAN_PCI,
        NULL, 0,                      // входные данные отсутствуют
        devices, sizeof(devices),     // выход: массив структур
        &bytesReturned,
        NULL);

    if (!ok) {
        printf("Ошибка при сканировании PCI (код %lu)\n", GetLastError());
        CloseHandle(hDevice);
        printf("Нажмите Enter для завершения...\n");
        (void)_getch();
        return 2;
    }

    // валидация протокола: размер должен быть кратен sizeof(PCI_DEVICE_INFO)
    if (bytesReturned % sizeof(PCI_DEVICE_INFO) != 0) {
        printf("Некорректный размер ответа драйвера (%lu байт)\n", bytesReturned);
        CloseHandle(hDevice);
        printf("Нажмите Enter для завершения...\n");
        (void)_getch();
        return 2;
    }

    // вычислить количество полученных элементов
    const int deviceCount = (int)(bytesReturned / sizeof(PCI_DEVICE_INFO));

    // открыть pci.ids (аргумент командной строки либо .\pci.ids)
    const wchar_t* pciIdsPath = (argc >= 2) ? argv[1] : defaultPciIds;
    FILE* pciFile = NULL;
    _wfopen_s(&pciFile, pciIdsPath, L"r");  // чтение 

    if (!pciFile) {
        printf("Не удалось открыть файл pci.ids по пути: ");
        wprintf(L"%ls\n", pciIdsPath);
        CloseHandle(hDevice);
        printf("Нажмите Enter для завершения...\n");
        (void)_getch();
        return 1;
    }

    // печать шапки таблицы
    printf("Сканирование шины PCI завершено. Найдено %d устройств:\n", deviceCount);
    const int colTitle = 64;                              // ширина колонки “Название”
    const int totalWidth = colTitle + 3 + 10 + 3 + 10 + 1;  // суммарная ширина строки
    printSeparator(totalWidth);
    printf("%-*s | %-10s | %-10s\n", colTitle, "Название", "Vendor ID", "Device ID");
    printSeparator(totalWidth);

    // печать строк таблицы
    for (int i = 0; i < deviceCount; ++i) {
        const char* title = getDeviceInfo(devices[i], pciFile);

        // визуальная обрезка названия до ширины colTitle
        char clipped[512];
        strncpy_s(clipped, sizeof(clipped), title, _TRUNCATE);
        if ((int)strlen(clipped) > colTitle) {
            clipped[colTitle - 3] = '.';
            clipped[colTitle - 2] = '.';
            clipped[colTitle - 1] = '.';
            clipped[colTitle] = '\0';
        }

        // вывод поля: Название | VendorID | DeviceID
        printf("%-*s | %04X       | %04X\n",
            colTitle, clipped, devices[i].vendorID, devices[i].deviceID);
    }

    // низ таблицы и корректное завершение
    printSeparator(totalWidth);
    fclose(pciFile);
    CloseHandle(hDevice);

    printf("Нажмите Enter для завершения...\n");
    (void)_getch();
    return 0;
}
