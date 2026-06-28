
#include "staticapi.h"
#include <windows.h>

void _staticFunction(char* message) {
    MessageBoxA(NULL, message, "My Static API App", MB_OK);
}