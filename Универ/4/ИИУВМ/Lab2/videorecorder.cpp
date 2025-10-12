#include "videorecorder.h"

#include <comdef.h>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <cwctype>

// для регистронезависимого поиска в строках DevicePath/InstanceID.
static std::wstring toLower(std::wstring s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](wchar_t c) { return static_cast<wchar_t>(::towlower(c)); });
    return s;
}

// пытаемся вытащить VID/PID из строки пути устройства.
// Ищем подстроки "vid_" и "pid_" (без учета регистра), читаем по 4 шестнадцатеричных символа после них.
// Возвращаем true, если получилось извлечь хотя бы одно из значений.
bool VideoRecorder::tryParseVidPid(const std::wstring& devicePath,
    std::wstring& outVid,
    std::wstring& outPid) {
    const std::wstring low = toLower(devicePath);
    const std::wstring vtag = L"vid_";
    const std::wstring ptag = L"pid_";

    const size_t vpos = low.find(vtag);
    const size_t ppos = low.find(ptag);

    if (vpos != std::wstring::npos && vpos + vtag.size() + 4 <= low.size()) {
        outVid = low.substr(vpos + vtag.size(), 4);
    }
    if (ppos != std::wstring::npos && ppos + ptag.size() + 4 <= low.size()) {
        outPid = low.substr(ppos + ptag.size(), 4);
    }
    return !outVid.empty() || !outPid.empty();
}

// инициализация COM, создание графа DirectShow, поиск первой доступной камеры.
// 1) CoInitializeEx — многопоточная модель COM.
// 2) Создаем FilterGraph и CaptureGraphBuilder2.
// 3) Через SystemDeviceEnum перечисляем устройства класса CLSID_VideoInputDeviceCategory.
// 4) Берем первый IMoniker (первая камера), читаем из него свойства (FriendlyName/DevicePath).
// 5) Привязываем монникер к IBaseFilter (источник) и добавляем его в граф.
// 6) Дополняем сведения о камере через SetupAPI (производитель/InstanceID).
bool VideoRecorder::initCaptureGraph() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::cerr << "COM init failed\n";
        return false;
    }

    hr = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraph);
    if (FAILED(hr)) {
        std::cerr << "CLSID_FilterGraph failed\n";
        CoUninitialize();
        return false;
    }

    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, nullptr, CLSCTX_INPROC_SERVER,
        IID_ICaptureGraphBuilder2, (void**)&pCapture);
    if (FAILED(hr)) {
        std::cerr << "CLSID_CaptureGraphBuilder2 failed\n";
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Привязываем builder к нашему графу
    pCapture->SetFiltergraph(pGraph);

    // Перечислитель устройств захвата видео
    ICreateDevEnum* pDevEnum = nullptr;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pDevEnum);
    if (FAILED(hr)) {
        std::cerr << "SystemDeviceEnum failed\n";
        SAFE_RELEASE(pCapture);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Получаем список видеовходов
    IEnumMoniker* pEnum = nullptr;
    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
    SAFE_RELEASE(pDevEnum);
    if (hr != S_OK || !pEnum) {
        std::cerr << "No video capture devices\n";
        SAFE_RELEASE(pCapture);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Берем первую доступную камеру
    IMoniker* pMoniker = nullptr;
    hr = pEnum->Next(1, &pMoniker, nullptr);
    SAFE_RELEASE(pEnum);
    if (hr != S_OK || !pMoniker) {
        std::cerr << "No camera moniker\n";
        SAFE_RELEASE(pCapture);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Читаем из монникера имя и VID/PID (через IPropertyBag: FriendlyName/DevicePath)
    saveCameraInfo(pMoniker);

    // Превращаем монникер в фильтр-источник камеры (IBaseFilter)
    hr = pMoniker->BindToObject(nullptr, nullptr, IID_IBaseFilter, (void**)&pSource);
    SAFE_RELEASE(pMoniker);
    if (FAILED(hr) || !pSource) {
        std::cerr << "BindToObject (camera) failed\n";
        SAFE_RELEASE(pCapture);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Добавляем источник в граф фильтров
    hr = pGraph->AddFilter(pSource, L"Video Capture");
    if (FAILED(hr)) {
        std::cerr << "AddFilter(camera) failed\n";
        SAFE_RELEASE(pSource);
        SAFE_RELEASE(pCapture);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return false;
    }

    // Дополняем карточку устройства через SetupAPI (Manufacturer, InstanceID),
    // сопоставляя по VID/PID или по имени.
    enrichInfoWithSetupAPI();
    return true;
}

// формируем цепочку захвата и запускаем граф.
// 1) Генерируем имя файла с таймштампом.
// 2) Через CaptureGraphBuilder2 создаем AVI Mux + File Writer (SetOutputFileName).
// 3) Рендерим видеопоток захвата в мультиплексор (RenderStream).
// 4) Получаем IMediaControl и запускаем граф (Run).
// В случае ошибки — аккуратно чистим созданные объекты.
bool VideoRecorder::startRecording() {
    if (recording || !pGraph || !pCapture || !pSource) {
        return false;
    }

    const std::wstring fileName = generateFileName();

    HRESULT hr = pCapture->SetOutputFileName(&MEDIASUBTYPE_Avi,
        fileName.c_str(),
        &pMux,
        &pSink);
    if (FAILED(hr) || !pMux || !pSink) {
        std::cerr << "SetOutputFileName failed\n";
        clearRecordingObjects();
        return false;
    }

    hr = pCapture->RenderStream(&PIN_CATEGORY_CAPTURE,
        &MEDIATYPE_Video,
        pSource,
        nullptr,
        pMux);
    if (FAILED(hr)) {
        std::cerr << "RenderStream failed\n";
        clearRecordingObjects();
        return false;
    }

    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    if (FAILED(hr) || !pControl) {
        std::cerr << "QueryInterface(IMediaControl) failed\n";
        clearRecordingObjects();
        return false;
    }

    hr = pControl->Run();
    if (FAILED(hr)) {
        std::cerr << "Graph Run failed\n";
        clearRecordingObjects();
        return false;
    }

    recording = true;
    std::wcout << L"Запись начата. Файл сохранен как: "
        << fileName << std::endl;
    return true;
}

// останавливаем граф и освобождаем объекты,
// связанные именно с записью (IMediaControl/IFileSinkFilter/AVI Mux).
bool VideoRecorder::stopRecording() {
    if (!recording) {
        return false;
    }

    if (pControl) {
        pControl->Stop();
    }

    clearRecordingObjects();
    recording = false;

    std::wcout << L"Запись была успешно остановлена." << std::endl;
    return true;
}

// локальная «уборка» после записи.
// Удаляем mux из графа (если добавлялся) и освобождаем pControl/pSink/pMux.
void VideoRecorder::clearRecordingObjects() {
    if (pGraph && pMux) {
        pGraph->RemoveFilter(pMux);
    }
    SAFE_RELEASE(pControl);
    SAFE_RELEASE(pSink);
    SAFE_RELEASE(pMux);
}

// финальная очистка перед выходом/уничтожением объекта.
// Останавливаем запись при необходимости, удаляем источник из графа и
// освобождаем все COM-объекты. Завершаем COM с CoUninitialize().
void VideoRecorder::cleanUp() {
    if (recording) {
        stopRecording();
    }
    if (pGraph && pSource) {
        pGraph->RemoveFilter(pSource);
    }
    SAFE_RELEASE(pSource);
    SAFE_RELEASE(pCapture);
    SAFE_RELEASE(pGraph);
    CoUninitialize();
} 

VideoRecorder::~VideoRecorder() {
    cleanUp();
}

// имя файла вида capture_YYYY-M-D_H-M-S.avi.
// Используем локальное время машины.
std::wstring VideoRecorder::generateFileName() const {
    std::wstringstream wss;
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);

    wss << L"capture_"
        << (1900 + tm.tm_year) << L"-"
        << (1 + tm.tm_mon) << L"-"
        << tm.tm_mday << L"_"
        << tm.tm_hour << L"-"
        << tm.tm_min << L"-"
        << tm.tm_sec << L".avi";
    return wss.str();
}

// печать сводной информации о найденной камере.
// Если какое-то поле не удалось определить — выводим «—».
void VideoRecorder::showCameraInfo() const {
    std::wcout << L"Информация о веб-камере:\n";
    std::wcout << L"\tНазвание устройства: "
        << (name.empty() ? L"—" : name) << L"\n";
    std::wcout << L"\tПроизводитель: "
        << (manufacturer.empty() ? L"—" : manufacturer) << L"\n";
    std::wcout << L"\tInstance ID: "
        << (instanceId.empty() ? L"—" : instanceId) << L"\n";
    std::wcout << L"\tVendor ID: "
        << (vid.empty() ? L"—" : vid) << L"\n";
    std::wcout << L"\tProduct ID: "
        << (pid.empty() ? L"—" : pid) << L"\n\n";
}

// читаем свойства монникера через IPropertyBag.
// FriendlyName -> name, DevicePath -> парсим VID/PID (регистронезависимо).
void VideoRecorder::saveCameraInfo(IMoniker* pMoniker) {
    IPropertyBag* pProp = nullptr;
    HRESULT hr = pMoniker->BindToStorage(nullptr,
        nullptr,
        IID_IPropertyBag,
        (void**)&pProp);
    if (SUCCEEDED(hr) && pProp) {
        VARIANT var;
        VariantInit(&var);

        // FriendlyName — отображаемое имя устройства
        if (SUCCEEDED(pProp->Read(NAME, &var, nullptr)) &&
            var.vt == VT_BSTR && var.bstrVal) {
            name = var.bstrVal;
        }
        VariantClear(&var);

        // DevicePath может содержать VID/PID; аккуратно парсим 4-символьные коды.
        if (SUCCEEDED(pProp->Read(DEVICEPATH, &var, nullptr)) &&
            var.vt == VT_BSTR && var.bstrVal) {
            const std::wstring path = var.bstrVal;
            std::wstring v, p;
            if (tryParseVidPid(path, v, p)) {
                if (!v.empty()) vid = v;
                if (!p.empty()) pid = p;
            }
        }
        VariantClear(&var);
    }
    SAFE_RELEASE(pProp);
}

// добираем Manufacturer и InstanceID через SetupAPI.
// Ищем в классе GUID_DEVCLASS_CAMERA, сопоставляем найденные устройства
// с нашей камерой по VID/PID (или, если не удалось, по FriendlyName).
void VideoRecorder::enrichInfoWithSetupAPI() {
    HDEVINFO h = SetupDiGetClassDevsW(&GUID_DEVCLASS_CAMERA,
        nullptr,
        nullptr,
        DIGCF_PRESENT);
    if (h == INVALID_HANDLE_VALUE) {
        return;
    }

    auto lower = [](std::wstring s) {
        std::transform(s.begin(), s.end(), s.begin(), ::towlower);
        return s;
    };

    for (DWORD i = 0;; ++i) {
        SP_DEVINFO_DATA info{};
        info.cbSize = sizeof(info);

        if (!SetupDiEnumDeviceInfo(h, i, &info)) {
            if (GetLastError() == ERROR_NO_MORE_ITEMS) {
                break;
            }
            continue;
        }

        // Полный Instance ID
        wchar_t inst[512]{};
        DWORD  need = 0;
        if (!SetupDiGetDeviceInstanceIdW(h, &info, inst, 512, &need)) {
            inst[0] = L'\0';
        }

        // Имя по возможности берем FriendlyName, иначе DeviceDesc.
        wchar_t nameW[256]{};
        DWORD  dt = 0;
        if (!SetupDiGetDeviceRegistryPropertyW(h,
            &info,
            SPDRP_FRIENDLYNAME,
            &dt,
            (PBYTE)nameW,
            sizeof(nameW),
            nullptr)) {
            SetupDiGetDeviceRegistryPropertyW(h,
                &info,
                SPDRP_DEVICEDESC,
                &dt,
                (PBYTE)nameW,
                sizeof(nameW),
                nullptr);
        }

        // Производитель (MFG) — может быть пустым на некоторых драйверах
        wchar_t mfgW[256]{};
        dt = 0;
        SetupDiGetDeviceRegistryPropertyW(h,
            &info,
            SPDRP_MFG,
            &dt,
            (PBYTE)mfgW,
            sizeof(mfgW),
            nullptr);

        // сначала по VID/PID в InstanceID (надежнее),
        // если их не было — пробуем сравнить имя с FriendlyName.
        std::wstring instLow = lower(inst ? inst : L"");
        bool match = true;

        if (!vid.empty()) {
            const std::wstring needle = lower(L"vid_" + vid);
            match &= (instLow.find(needle) != std::wstring::npos);
        }
        if (!pid.empty()) {
            const std::wstring needle = lower(L"pid_" + pid);
            match &= (instLow.find(needle) != std::wstring::npos);
        }
        if (!match && !name.empty() && nameW[0]) {
            match = (lower(name) == lower(nameW));
        }

        // Если это нужный девайс — запоминаем Manufacturer/InstanceID,
        // а также переопределяем name, если он был пуст.
        if (match) {
            if (manufacturer.empty() && mfgW[0]) {
                manufacturer = mfgW;
            }
            if (instanceId.empty() && inst[0]) {
                instanceId = inst;
            }
            if (name.empty() && nameW[0]) {
                name = nameW;
            }
            break;
        }
    }

    SetupDiDestroyDeviceInfoList(h);
}
