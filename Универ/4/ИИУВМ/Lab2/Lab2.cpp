#include <windows.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <cstdio>

#include "videorecorder.h"

#define START_KEY VK_SPACE
#define STOP_KEY VK_ESCAPE
#define EXIT_KEY 0x51    // 'Q'
#define TOGGLE_CONSOLE 0x57    // 'W' 
 
static HHOOK gKeyboardHook = nullptr;
static VideoRecorder gRecorder;
static bool gConsoleVisible = true;

static void setupConsoleUtf16() {
    if (GetConsoleWindow()) {
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
        _setmode(_fileno(stdin), _O_U16TEXT);
    }
}

static void printManual() {
    std::wcout << L"Краткое руководство:\n";
    const int w = 14;
    std::wcout << std::left << std::setw(w) << L"\tПробел" << L"- начать запись\n";
    std::wcout << std::left << std::setw(w) << L"\tEsc" << L"- остановить запись\n";
    std::wcout << std::left << std::setw(w) << L"\tCtrl + Q" << L"- выйти\n";
    std::wcout << std::left << std::setw(w) << L"\tCtrl + W" << L"- показать/скрыть консоль\n";  
}

static void toggleConsole() {
    HWND h = GetConsoleWindow();
    if (!h) {
        return;
    }
    ShowWindow(h, gConsoleVisible ? SW_HIDE : SW_SHOW);
    gConsoleVisible = !gConsoleVisible;
}

static void detachConsoleSoft() {
    HWND h = GetConsoleWindow();
    if (h) {
        ShowWindow(h, SW_HIDE);
    }

    FreeConsole();

    FILE* f_out = nullptr;
    FILE* f_err = nullptr;
    FILE* f_in = nullptr;

    freopen_s(&f_out, "NUL", "w", stdout);
    freopen_s(&f_err, "NUL", "w", stderr);
    freopen_s(&f_in, "NUL", "r", stdin);

    SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
    SetStdHandle(STD_ERROR_HANDLE, nullptr);
    SetStdHandle(STD_INPUT_HANDLE, nullptr);

    gConsoleVisible = false;
}

static LRESULT CALLBACK LowLevelKeyboardProc(int nCode,
    WPARAM wParam,
    LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        const auto vk = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam)->vkCode;

        switch (vk) {
        case START_KEY:
            gRecorder.startRecording();
            break;

        case STOP_KEY:
            gRecorder.stopRecording();
            break;

        case EXIT_KEY:
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                PostQuitMessage(0);
            }
            break;

        case TOGGLE_CONSOLE:
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                toggleConsole();
            }
            break;

        default:
            break;
        }
    }

    return CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);
}

static void enterHiddenModeIfRequested(int argc, wchar_t** argv) {
    for (int i = 1; i < argc; ++i) {
        if (wcscmp(argv[i], L"--hidden") == 0) {
            detachConsoleSoft();
            break;
        }
    }
}

int wmain(int argc, wchar_t** argv) {
    enterHiddenModeIfRequested(argc, argv);
    setupConsoleUtf16();

    if (!gRecorder.initCaptureGraph()) {
        std::wcerr << L"Инициализация камеры не удалась\n";
        return 1;
    }

    if (GetConsoleWindow()) {
        printManual();
        gRecorder.showCameraInfo();
    }

    gKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        nullptr,
        0);
    if (!gKeyboardHook) {
        std::wcerr << L"Не удалось установить перехватчик клавиатуры\n";
        gRecorder.cleanUp();
        return 2;
    }

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnhookWindowsHookEx(gKeyboardHook);
    gRecorder.cleanUp();
    return 0;
}



//int wmain(int argc, wchar_t** argv) {
   // detachConsoleSoft();
    // enterHiddenModeIfRequested(argc, argv); 
    // setupConsoleUtf16();                  

    //if (!gRecorder.initCaptureGraph()) {
      //  return 1;
    //}

    //gKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    //if (!gKeyboardHook) {
      //  gRecorder.cleanUp();
        //return 2;
    //}

    //MSG msg;
    //while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
      //  TranslateMessage(&msg);
        //DispatchMessageW(&msg);
    //}
    
    //UnhookWindowsHookEx(gKeyboardHook);
    //gRecorder.cleanUp();
    //return 0;
//}
