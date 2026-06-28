#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <intrin.h>

int main(void) {
    uint64_t t = __rdtsc();

    printf("%X\n", t);

    __halt();

    __debugbreak();

    __fastfail(FAST_FAIL_CORRUPT_LIST_ENTRY);

    uint16_t port = 0x378;
    uint8_t data;
    data  = __inbyte(port);
    __outbyte(port, data | 0x01);
    
}