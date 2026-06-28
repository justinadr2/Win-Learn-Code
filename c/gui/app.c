#define UNICODE
#include <windows.h>

#define ID_BUTTON_HELLO  1
#define ID_BUTTON_EXIT   2

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindowW(L"BUTTON", L"Say Hello", WS_VISIBLE | WS_CHILD, 50, 50, 120, 30, hwnd, (HMENU)ID_BUTTON_HELLO, NULL, NULL);

        CreateWindowW(L"BUTTON", L"Exit", WS_VISIBLE | WS_CHILD, 50, 100, 120, 30, hwnd, (HMENU)ID_BUTTON_EXIT, NULL, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_HELLO: MessageBoxW(hwnd, L"Hello from Win32!", L"Message", MB_OK); 
        break;

        case ID_BUTTON_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {0};

    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"MyWindowClass";

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, L"MyWindowClass", L"Simple Win32 Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
        CW_USEDEFAULT, 300, 250, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
