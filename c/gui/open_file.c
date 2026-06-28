#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

#define ID_OPEN_BUTTON  1001
#define ID_EDIT_OUTPUT  1002

HWND g_hEdit;

void OpenAndDisplayFile(HWND hwnd) {
    OPENFILENAMEA ofn;
    char filePath[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = hwnd;
    ofn.lpstrFile   = filePath;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameA(&ofn))
        return;

    HANDLE hFile = CreateFileA(
        filePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBoxA(hwnd, "Failed to open file", "Error", MB_ICONERROR);
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        CloseHandle(hFile);
        MessageBoxA(hwnd, "Invalid or empty file", "Error", MB_ICONERROR);
        return;
    }

    char* buffer = (char*)HeapAlloc(GetProcessHeap(), 0, fileSize + 1);
    if (!buffer) {
        CloseHandle(hFile);
        return;
    }

    DWORD bytesRead = 0;
    ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
    CloseHandle(hFile);

    buffer[bytesRead] = '\0';

    // convert non-printable ASCII to '.'
    for (DWORD i = 0; i < bytesRead; i++) {
        unsigned char c = buffer[i];
        if (c < 32 || c > 126)
            buffer[i] = '.';
    }

    SetWindowTextA(g_hEdit, buffer);

    HeapFree(GetProcessHeap(), 0, buffer);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowA(
            "BUTTON", "Open File",
            WS_VISIBLE | WS_CHILD,
            10, 10, 100, 28,
            hwnd, (HMENU)ID_OPEN_BUTTON,
            NULL, NULL
        );

        g_hEdit = CreateWindowA(
            "EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER |
            ES_MULTILINE | ES_AUTOVSCROLL |
            ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL,
            10, 50, 760, 500,
            hwnd, (HMENU)ID_EDIT_OUTPUT,
            NULL, NULL
        );
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_OPEN_BUTTON) {
            OpenAndDisplayFile(hwnd);
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
    wc.lpszClassName = "AsciiViewer";

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        "AsciiViewer",
        "ASCII File Viewer (C / Win32)",
        WS_OVERLAPPEDWINDOW,
        100, 100, 800, 620,
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
