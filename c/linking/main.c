#include <stdio.h>
#include "staticapi.h"

int main(void) {
    printf("your message: ");
    char string[32];

    scanf("%31s", string);
    
    _staticFunction(string);

    return 0;
}