#define DYNAMICAPI_EXPORTS
#include "dynamicapi.h"
#include <windows.h>


void _dynamicFunction(char* message) {
    MessageBoxA(NULL, message, "My Dynamic API App", MB_OK);
}