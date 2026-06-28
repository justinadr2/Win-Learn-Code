#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hIn, hOut;
    char buffer[128];
    DWORD bytesRead = 0, bytesWritten = 0;

    hIn = GetStdHandle(STD_INPUT_HANDLE);

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    ReadFile(hIn, buffer, sizeof(buffer), &bytesRead, NULL);

    WriteFile(hOut, buffer, bytesRead, &bytesWritten, NULL);
}