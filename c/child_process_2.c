#include <windows.h>
#include <stdio.h>
#include <string.h>

PROCESS_INFORMATION StartChild(char* cmdLine) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        printf("[PARENT] CreateProcess failed (%lu).\n", GetLastError());
    }
    return pi;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        DWORD myPid = GetCurrentProcessId();
        const char* parentPid = argv[2];

        if (strcmp(argv[1], "--type=utility") == 0) {
            printf("[UTILITY] My PID: %lu\n", myPid);
            printf("[UTILITY] Parent PID: %s\n", parentPid);
            printf("[UTILITY] Utilizing algorithm...\n");
            printf("Press ENTER to exit.\n");
            getchar(); 
            return 0;
        } 
        else if (strcmp(argv[1], "--type=renderer") == 0) {
            printf("[RENDERER] My PID: %lu\n", myPid);
            printf("[RENDERER] Parent PID: %s\n", parentPid);
            printf("[RENDERER] Rendering triangle...\n");
            printf("Press ENTER to exit.\n");
            getchar();
            return 0;
        }
    }

    char workerCmd[MAX_PATH];
    char gpuCmd[MAX_PATH];
    DWORD parentPid = GetCurrentProcessId();

    sprintf(workerCmd, "%s --type=utility %lu", argv[0], parentPid);
    sprintf(gpuCmd, "%s --type=renderer %lu", argv[0], parentPid);

    PROCESS_INFORMATION children[2];
    HANDLE waitHandles[2];

    printf("[PARENT] My PID: %lu\n", parentPid);
    
    children[0] = StartChild(workerCmd);
    children[1] = StartChild(gpuCmd);

    if (children[0].hProcess == NULL || children[1].hProcess == NULL) {
        printf("[PARENT] Error launching children.\n");
        return 1;
    }

    printf("[PARENT] Spawning UTILITY  (%lu)\n", children[0].dwProcessId);
    printf("[PARENT] Spawning RENDERER (%lu)\n", children[1].dwProcessId);

    waitHandles[0] = children[0].hProcess;
    waitHandles[1] = children[1].hProcess;

    int processesRunning = 2;
    printf("[PARENT] Monitoring children for termination...\n\n");

    while (processesRunning > 0) {
        DWORD result = WaitForMultipleObjects(processesRunning, waitHandles, FALSE, INFINITE);
        DWORD index = result - WAIT_OBJECT_0;

        if (index < (DWORD)processesRunning) {
            if (waitHandles[index] == children[0].hProcess) {
                printf("[NOTIF] The UTILITY process (%lu) has terminated.\n", children[0].dwProcessId);
            } 
            else if (waitHandles[index] == children[1].hProcess) {
                printf("[NOTIF] The RENDERER process (%lu) has terminated.\n", children[1].dwProcessId);
            }

            for (DWORD i = index; i < (DWORD)processesRunning - 1; i++) {
                waitHandles[i] = waitHandles[i + 1];
            }
            processesRunning--;
        }
    }

    printf("\n[PARENT] Both children finished. Press ENTER to close Main.\n");
    
    rewind(stdin);
    getchar(); 

    for(int i = 0; i < 2; i++) {
        CloseHandle(children[i].hProcess);
        CloseHandle(children[i].hThread);
    }

    return 0;
}