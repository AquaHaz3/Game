// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

static HMODULE __main_module = 0;

extern "C" __declspec(dllexport) void SetMainModule(HMODULE module) {
    __main_module = module;
}

extern "C" __declspec(dllexport) HMODULE GetMainModule() {
    return __main_module;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

