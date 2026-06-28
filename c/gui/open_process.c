#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

#define ID_BUTTON_OPEN  1001
#define ID_STATIC_PID   1002

HWND gStaticPid;

void LaunchExeAndShowPid(HWND hwnd) {
    OPENFILENAMEA ofn;
    char filePath[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = hwnd;
    ofn.lpstrFile   = filePath;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrFilter = "Executables (*.exe)\0*.exe\0All Files\0*.*\0";
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_EXPLORER;

    if (!GetOpenFileNameA(&ofn))
        return;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(filePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        char buf[128];
        sprintf(buf, "Launched %s\nProcess ID: %lu", filePath, pi.dwProcessId);
        SetWindowTextA(gStaticPid, buf);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        MessageBoxA(hwnd, "Failed to launch process", "Error", MB_ICONERROR);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindowA(
            "BUTTON", "Open .exe and Launch",
            WS_VISIBLE | WS_CHILD,
            10, 10, 200, 28,
            hwnd, (HMENU)ID_BUTTON_OPEN,
            NULL, NULL
        );

        gStaticPid = CreateWindowA(
            "STATIC", "Process ID: ",
            WS_VISIBLE | WS_CHILD,
            10, 50, 400, 40,
            hwnd, (HMENU)ID_STATIC_PID,
            NULL, NULL
        );
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_OPEN) {
            LaunchExeAndShowPid(hwnd);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "ExeLauncher";

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        "ExeLauncher",
        "EXE Launcher – Show PID",
        WS_OVERLAPPEDWINDOW,
        100, 100, 500, 170,
        NULL, NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}
