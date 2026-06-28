#include <windows.h>
#include <stdio.h>

int main() {    
    LPVOID lpAddress1 = VirtualAlloc(NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("Region 1 allocated at: 0x%p\n", lpAddress1);

    unsigned char* buf1 = (unsigned char*)lpAddress1;
    buf1[0] = 0xDE; 
    buf1[1] = 0xAD; 
    buf1[2] = 0xBE; 
    buf1[3] = 0xEF;
    printf("Region 1 Data: 0x%02X%02X%02X%02X\n\n", buf1[0], buf1[1], buf1[2], buf1[3]);

    LPVOID lpAddress2 = VirtualAlloc(NULL, 0x2000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("Region 2 allocated at: 0x%p (Size: 0x1500)\n", lpAddress2);

    unsigned char* buf2 = (unsigned char*)lpAddress2;
    buf2[0] = 0xCA;
    buf2[1] = 0xFE;
    buf2[2] = 0xBA;
    buf2[3] = 0xBE;

    printf("Region 2 Data: 0x%02X%02X%02X%02X\n", buf2[0], buf2[1], buf2[2], buf2[3]);

    printf("\nPress Enter to release memory and exit...");
    
    VirtualFree(lpAddress1, 0, MEM_RELEASE);
    VirtualFree(lpAddress2, 0, MEM_RELEASE);
    
    return 0;
}