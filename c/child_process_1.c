#include <windows.h>
#include <stdio.h>
#include <string.h>

void RunChild(const char* type, HANDLE hIn, HANDLE hOut, int parentPid) {
    char buffer[256] = {0};
    char response[256] = {0};
    DWORD bytesRead, bytesWritten;

    printf("[%s] My PID: %lu\n", type, GetCurrentProcessId());
    printf("[%s] Parent PID: %d\n", type, parentPid);
    printf("[%s] Receiving commands...\n", type);
    
    if (ReadFile(hIn, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        printf("[%s] Parent said: \"%s\"\n", type, buffer);

        if (strstr(buffer, "utilize-algorithm")) {
            strcpy(response, "Utilizing algorithm...");
        }
        else if (strstr(buffer, "render-triangle")) {
            strcpy(response, "Rendering triangle...");
        } 
        else {
            strcpy(response, "[CHILD] Message received, but no command recognized.");
        }

        WriteFile(hOut, response, (DWORD)strlen(response) + 1, &bytesWritten, NULL);
    }

    printf("[%s] Task finished. Press Enter to exit.\n", type);
    getchar();
}

int main(int argc, char* argv[]) {    
    if (argc > 4) {
        HANDLE hIn = (HANDLE)atoi(argv[2]);
        HANDLE hOut = (HANDLE)atoi(argv[3]);
        int parentPid = atoi(argv[4]);

        if (strstr(argv[1], "--type=utility")) 
            RunChild("UTILITY", hIn, hOut, parentPid);
        else if (strstr(argv[1], "--type=renderer")) 
            RunChild("RENDERER", hIn, hOut, parentPid);
        return 0;
    }

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    HANDLE hW_In_R, hW_In_W;   
    HANDLE hW_Out_R, hW_Out_W; 
    HANDLE hG_In_R, hG_In_W;   
    HANDLE hG_Out_R, hG_Out_W; 

    CreatePipe(&hW_In_R, &hW_In_W, &sa, 0);
    CreatePipe(&hW_Out_R, &hW_Out_W, &sa, 0);
    CreatePipe(&hG_In_R, &hG_In_W, &sa, 0);
    CreatePipe(&hG_Out_R, &hG_Out_W, &sa, 0);

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION piW = {0}, piG = {0};
    char utilityCmd[512], rendererCmd[512];

    DWORD mainPid = GetCurrentProcessId();
    printf("[PARENT] My PID is: %lu\n", mainPid);

    sprintf(utilityCmd, "%s --type=utility %d %d %lu", argv[0], (int)hW_In_R, (int)hW_Out_W, mainPid);
    sprintf(rendererCmd, "%s --type=renderer %d %d %lu", argv[0], (int)hG_In_R, (int)hG_Out_W, mainPid);

    CreateProcess(NULL, utilityCmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piW);
    CreateProcess(NULL, rendererCmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piG);

    printf("[PARENT] Spawned UTILITY (%lu)\n", piW.dwProcessId);
    printf("[PARENT] Spawned RENDERER (%lu)\n\n", piG.dwProcessId);

    DWORD written, read;
    char reply[256];

    printf("[PARENT] Sending commands...\n");
    WriteFile(hW_In_W, "utilize-algorithm", 17, &written, NULL);
    WriteFile(hG_In_W, "render-triangle", 15, &written, NULL);

    ReadFile(hW_Out_R, reply, sizeof(reply), &read, NULL);
    printf("[PARENT] Utility replied: %s\n", reply);

    ReadFile(hG_Out_R, reply, sizeof(reply), &read, NULL);
    printf("[PARENT] Renderer replied: %s\n", reply);

    printf("\n[PARENT] All replies received. Press Enter to exit.\n");
    
    CloseHandle(piW.hProcess); CloseHandle(piW.hThread);
    CloseHandle(piG.hProcess); CloseHandle(piG.hThread);

    getchar();
    return 0;
}   