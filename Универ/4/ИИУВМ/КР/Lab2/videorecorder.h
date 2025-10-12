#pragma once

#include <dshow.h>
#include <setupapi.h>
#include <devguid.h>
#include <string>
#include <iostream>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "setupapi.lib")

#define NAME L"FriendlyName"
#define DEVICEPATH L"DevicePath"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) do { if (p) { (p)->Release(); (p) = nullptr; } } while (0)
#endif

class VideoRecorder {
public:
    bool initCaptureGraph();      // Инициализация COM, поиск камеры, сборка графа
    bool startRecording();        // Начать запись в AVI
    bool stopRecording();         // Остановить запись
    void showCameraInfo() const;  // Имя, производитель, InstanceID, VID/PID
    void cleanUp();               // Очистка
    ~VideoRecorder();

private:
    std::wstring generateFileName() const;
    void saveCameraInfo(IMoniker* pMoniker);  // FriendlyName + VID/PID из DevicePath
    static bool tryParseVidPid(const std::wstring& devicePath,
        std::wstring& outVid,
        std::wstring& outPid);
    void enrichInfoWithSetupAPI();            // Manufacturer + InstanceID
    void clearRecordingObjects();             // Чистка pControl/pSink/pMux
     
private:
    IGraphBuilder* pGraph = nullptr;
    ICaptureGraphBuilder2* pCapture = nullptr;
    IMediaControl* pControl = nullptr;
    IBaseFilter* pSource = nullptr;  // камера
    IFileSinkFilter* pSink = nullptr;  // File Writer
    IBaseFilter* pMux = nullptr;  // AVI Mux

    bool recording = false;

    std::wstring name;          // FriendlyName
    std::wstring vid;           // 4 hex
    std::wstring pid;           // 4 hex
    std::wstring manufacturer;  // SetupAPI
    std::wstring instanceId;    // SetupAPI
};
