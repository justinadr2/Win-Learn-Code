#include <windows.h>

void SetCurrentThreadName(const wchar_t* name) {
    SetThreadDescription(GetCurrentThread(), name);
}

void Worker() {
    SetCurrentThreadName(L"Custom worker thread");

    printf("worker thread: %d\n", GetCurrentThreadId());
    
    return 0;
}

int main() {
    HANDLE thread;

    printf("main thread: %d\n", GetCurrentThreadId());

    thread = CreateThread(NULL, 0, Worker, NULL, 0, NULL);

    WaitForSingleObject(thread, INFINITE);

    CloseHandle(thread);

}
