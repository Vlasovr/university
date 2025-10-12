#include <ntddk.h>
#include <wdmsec.h>
#pragma comment(lib, "wdmsec.lib")

// Порты механизма конфигурации PCI #1
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

// IOCTL: запросить у драйвера сканирование PCI; метод — буферизованный
#define IOCTL_SCAN_PCI CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Контейнер одного результата: пара идентификаторов
typedef struct _PCI_DEVICE_INFO {
    UINT16 vendorID;  // Производитель (4 hex)
    UINT16 deviceID;  // Устройство   (4 hex)
} PCI_DEVICE_INFO;
 

// чтение DWORD из конфигурационного пространства устройства PCI
// вход: bus/slot/func/offset; выход: 32-битное значение регистра
static
ULONG PciConfigRead(ULONG bus, ULONG slot, ULONG func, ULONG offset)
{
    ULONG address =
        (1UL << 31) |     
        ((bus & 0xFF) << 16) |
        ((slot & 0x1F) << 11) |
        ((func & 0x07) << 8) |
        (offset & 0xFC);               // кратность 4 байтам

    WRITE_PORT_ULONG((PULONG)(ULONG_PTR)PCI_CONFIG_ADDRESS, address);
    return READ_PORT_ULONG((PULONG)(ULONG_PTR)PCI_CONFIG_DATA);
}

// чтение пары (VendorID, DeviceID) из регистра 0x00..0x03
// возвращает TRUE, если устройство существует (vendor != 0xFFFF)
static
BOOLEAN ReadVendorDevice(ULONG bus, ULONG slot, ULONG func, UINT16* vendor, UINT16* device)
{
    ULONG data = PciConfigRead(bus, slot, func, 0x00);
    *vendor = (UINT16)(data & 0xFFFF);
    *device = (UINT16)((data >> 16) & 0xFFFF);
    return (*vendor != 0xFFFF);
}

// чтение Header Type из регистра 0x0C..0x0F (байт [23:16])
// назначение: определить многофункциональность устройства 
static
UCHAR ReadHeaderType(ULONG bus, ULONG slot, ULONG func)
{
    ULONG data = PciConfigRead(bus, slot, func, 0x0C);
    return (UCHAR)((data >> 16) & 0xFF);
}

/* -------------------------- СКАНИРОВАНИЕ ШИНЫ PCI ------------------------ */

// обход: bus=0..255, slot=0..31, func=0..(1|8); запись результатов в out
// защита: не выходить за пределы capacity
static
VOID ScanPciBus(PCI_DEVICE_INFO* out, UINT32* outCount, UINT32 capacity)
{
    UINT32 count = 0;

    for (UINT32 bus = 0; bus < 256 && count < capacity; ++bus) {
        for (UINT32 slot = 0; slot < 32 && count < capacity; ++slot) {

            // сначала function 0; если нет устройства — слот пуст
            UINT16 v0, d0;
            if (!ReadVendorDevice(bus, slot, 0, &v0, &d0)) {
                continue;
            }

            // добавить функцию 0
            out[count].vendorID = v0;
            out[count].deviceID = d0;
            if (++count >= capacity) break;

            // определить, есть ли дополнительные функции 1..7
            UCHAR  hdr = ReadHeaderType(bus, slot, 0);
            BOOLEAN multi = (hdr & 0x80) ? TRUE : FALSE;
            UINT32 maxFunc = multi ? 8 : 1;

            // добавить остальные функции (если присутствуют)
            for (UINT32 func = 1; func < maxFunc && count < capacity; ++func) {
                UINT16 v, d;
                if (!ReadVendorDevice(bus, slot, func, &v, &d)) {
                    continue;
                }
                out[count].vendorID = v;
                out[count].deviceID = d;
                ++count;
            }
        }
    }

    *outCount = count;
}

/* ------------------------------- IRP-ОБРАБОТЧИКИ ------------------------- */

// обработка IRP_MJ_DEVICE_CONTROL; поддерживается только IOCTL_SCAN_PCI
static
NTSTATUS DeviceIoControlHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION io = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

    if (io->Parameters.DeviceIoControl.IoControlCode == IOCTL_SCAN_PCI) {
        if (!Irp->AssociatedIrp.SystemBuffer) {
            status = STATUS_INVALID_PARAMETER;               // отсутствие буфера
        }
        else {
            ULONG outLen = io->Parameters.DeviceIoControl.OutputBufferLength;
            ULONG capacity = (ULONG)(outLen / sizeof(PCI_DEVICE_INFO));

            if (capacity == 0) {
                status = STATUS_BUFFER_TOO_SMALL;            // нулевой размер
            }
            else {
                PCI_DEVICE_INFO* buf = (PCI_DEVICE_INFO*)Irp->AssociatedIrp.SystemBuffer;
                UINT32 count = 0;
                ScanPciBus(buf, &count, capacity);           // заполнение буфера
                Irp->IoStatus.Information = count * sizeof(PCI_DEVICE_INFO);
            }
        }
    }
    else {
        status = STATUS_INVALID_DEVICE_REQUEST;              // неизвестный IOCTL
    }

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

// обработка IRP_MJ_CREATE: подтверждение открытия устройства
static
NTSTATUS DispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

// обработка IRP_MJ_CLOSE: подтверждение закрытия устройства
static
NTSTATUS DispatchClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

// выгрузка драйвера: удалить симлинк и объект устройства
static
VOID UnloadDriver(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING symLink;
    RtlInitUnicodeString(&symLink, L"\\DosDevices\\PciScanner");
    IoDeleteSymbolicLink(&symLink);

    if (DriverObject->DeviceObject) {
        IoDeleteDevice(DriverObject->DeviceObject);
    }
}

/* --------------------------------- ВХОД ---------------------------------- */

// инициализация: создать устройство, симлинк, установить диспетчеры IRP
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;
    PDEVICE_OBJECT DeviceObject = NULL;
    UNICODE_STRING devName, symLink, sddl;

    // имена объекта и симлинка
    RtlInitUnicodeString(&devName, L"\\Device\\PciScanner");
    RtlInitUnicodeString(&symLink, L"\\DosDevices\\PciScanner");

    // SDDL: полный доступ SYSTEM и Administrators
    // чтение/запись пользователям не предоставлять
    RtlInitUnicodeString(&sddl, L"D:P(A;;GA;;;SY)(A;;GA;;;BA)");

    // создание устройства с политикой безопасности (wdmsec.lib)
    status = IoCreateDeviceSecure(
        DriverObject,
        0,                     // размер расширения устройства
        &devName,              // имя 
        FILE_DEVICE_UNKNOWN,   // тип устройства
        0,                     // характеристики
        FALSE,                 // не эксклюзивный
        &sddl,                 // SDDL-строка
        NULL,                
        &DeviceObject
    );
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // MODE: буферизованный ввод/вывод для METHOD_BUFFERED
    DeviceObject->Flags |= DO_BUFFERED_IO;

    // символьная ссылка для user-mode: \\.\PciScanner
    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(DeviceObject);
        return status;
    }

    // регистрация диспетчеров IRP
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceIoControlHandler;
    DriverObject->DriverUnload = UnloadDriver;

    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    return STATUS_SUCCESS;
}
