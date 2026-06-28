#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;
    DWORD bytesWritten, bytesRead;
    char buffer[1024];

    hFile = CreateFileA("code.bin", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    WriteFile(hFile, "set x 4;\nset y 5;\n", 18, &bytesWritten, NULL);
    CloseHandle(hFile);

    hFile = CreateFileA("code.bin", FILE_APPEND_DATA, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // Move pointer to the end for appending
    SetFilePointer(hFile, 0, NULL, FILE_END);

    WriteFile(hFile, "get x + y;\n", 11, &bytesWritten, NULL);
    CloseHandle(hFile);

    hFile = CreateFileA("code.bin", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    while (ReadFile(hFile, buffer, sizeof(buffer)-1, &bytesRead, NULL) && bytesRead > 0) 
    {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    CloseHandle(hFile);
}