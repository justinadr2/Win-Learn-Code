#include <stdio.h>
#include <windows.h>

#pragma section(".customsec", read, write)

__declspec(allocate(".customsec")) 
int my_custom_data = 0xDEADC0DE;

int main() {
    printf("Data value: 0x%X\n", my_custom_data);
    printf("Memory: %p\n", &my_custom_data);

    system("pause");
    return 0;
}