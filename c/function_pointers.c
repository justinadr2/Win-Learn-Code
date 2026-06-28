#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <intrin.h>

int sub(int a, int b) {
    return a - b;
}

int add(int a, int b) {
    return a + b;
}

int calc(int(*func)(int, int), int x, int y) {
    return func(x, y);
}

int main(int argc, char** argv) {
    printf("%i\n", calc(add, 4, 5));
    
    printf("%i\n", calc(sub, 10, 8));

    int(*addptr)(int, int) = add;

    int(**ptr_to_addptr)(int, int) = &addptr;

    int h = (*ptr_to_addptr)(10, 20);

    printf("%i\n", h);
}