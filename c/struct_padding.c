// why?
#include <stdio.h>
#include <windows.h>

#define GET_OFFSET(st, m) ((size_t)&(((st*)0)->m))

typedef struct Vec3_ {
    int x;
    int y;
    int z;
    // for 64-bit exe, the compiler adds a 4-byte padding here because the next property
    // takes 8-byte address and its address must start at a multiple of 8.
    // for 32-bit exe, theres no padding because both int and char* takes 4 bytes
    char* name;
} Vec3;

int main() {
    char name[] = "JUSTINADR";
    for (int i = 0; i < sizeof(name); i++)
        printf("%X = %X (%c)\n", &name[i], name[i], name[i]);

    printf("\n");
    
    int buffer[] = { 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD };
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
        printf("%X = %X\n", &buffer[i], buffer[i]);

    printf("\n");

    Vec3 vecs[] = {
        { 0x11, 0x22, 0x33, "positions"},
        { 0x44, 0x55, 0x66, "coordinates"}
    };

    printf("size of vecs = %i\n", sizeof(vecs));
    printf("size of Vec3 = %i\n", sizeof(Vec3));

    for (int i = 0; i < sizeof(vecs) / sizeof(vecs[0]); i++) {
        printf("%X = %X\n", &(*(vecs + i)).x, vecs[i].x);
        printf("%X = %X\n", &(*(vecs + i)).y, vecs[i].y);
        printf("%X = %X\n", &vecs[i].z, vecs[i].z);
        printf("%X = %s\n\n", &vecs[i].name, vecs[i].name);
    }
}