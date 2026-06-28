#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int** heap = (int**)malloc(sizeof(int*) * 16);

    heap[0] = (int*)malloc(sizeof(int));
    heap[1] = (int*)malloc(sizeof(int));
    
    *heap[0] = 0xdead;
    *heap[1] = 0xbeef;
    
    printf("%p = %i\n", heap[0], *heap[0]);
    printf("%p = %i\n", heap[1], *heap[1]);

    free(heap[0]);
    free(heap[1]);
    free(heap);

    /* */
    int a = 0xbaad;
    int b = 0xf00d;

    int* arr[2];
    int** stack = arr;

    stack[0] = &a;

    stack[1] = &b;

    printf("%p = %i\n", &stack[0], stack[0]);
    printf("%p = %i\n", &stack[1], stack[1]);
}