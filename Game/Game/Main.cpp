#include "GameLauncher.h"
#include <iostream>

int test(int argc, char** argv);
int init_external_access();

int main(int argc, char** argv)
{

    init_external_access();

    try {
        auto launcher = GameLauncher();
        launcher.Launch();
    }
    catch (std::exception error) {
        std::cout << "Fatal Error: " << error.what() << '\n';
    }
  
    return 0;
} 


#define WIN32_LEAN_AND_MEAN
#if defined(_WIN32)
#define WIN32
#endif
#if defined(_WIN64)
#define WIN64
#define _AMD64_
#undef _X86_
#else
#undef _AMD64_
#define _X86_
#endif

#include <minwindef.h>
#include <stdio.h>
#include <stdlib.h>
#include <libloaderapi.h>

#include "NativeBridge.hpp"

static HMODULE __native_bridge = 0;
typedef void (*unknown_func)(...);

int test(int argc, char** argv)
{
    return 0;
}

int init_external_access()
{
    __native_bridge = LoadLibraryA("NativeBridgeDLL");
    if (__native_bridge == 0) {
        printf("\nFatal Error: \n\tCan't load NativeBridgeDLL.dll");
        exit(0);
    }
    unknown_func setMain = (unknown_func) GetProcAddress(__native_bridge, "SetMainModule");
    HMODULE __this_EXE = 0;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR) init_external_access, &__this_EXE);
    setMain(__this_EXE);
    return 0;
}
