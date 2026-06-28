// for x86 only
#include <stdio.h>

int main() {
    int a = 5, b = 3, c;
    __asm {
        mov eax, a
        add eax, b
        mov c, eax
    }
    printf("c = %d\n", c);
    return 0;
}