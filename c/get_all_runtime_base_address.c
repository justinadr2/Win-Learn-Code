#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <string.h>
#include <intrin.h>

void printAllSectionBases(ULONG_PTR imageBase) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)imageBase;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(imageBase + dosHeader->e_lfanew);

    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
    
    printf("Listing sections for ImageBase: 0x%p\n", (void*)imageBase);
    printf("----------------------------------------------------------\n");
    printf("%-8s | %-18s | %-10s\n", "Name", "Runtime Base", "Virtual Size");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
        ULONG_PTR sectionBase = imageBase + section[i].VirtualAddress;

        printf("%-8.8s | 0x%-16p | 0x%-8X\n", section[i].Name, 
            (void*)sectionBase, section[i].Misc.VirtualSize);
    }
}

ULONG_PTR getImageBase()  {
    // intrinsic for "mov rax, gs:[offset]"
    ULONG_PTR pebAddr = __readgsqword(0x60);
    
    ULONG_PTR ldr = *(ULONG_PTR*)(pebAddr + 0x18);
    
    ULONG_PTR listEntry = *(ULONG_PTR*)(ldr + 0x10);
    
    ULONG_PTR imageBase = *(ULONG_PTR*)(listEntry + 0x30);

    printf("Runtime image base: 0x%p\n", (void*)imageBase);

    return imageBase;
}

int main() {
    printAllSectionBases(getImageBase());
}