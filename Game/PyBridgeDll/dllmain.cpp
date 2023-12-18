// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <vector>
#include <string>
#include <map>

void OnAttach() {

}

struct ArgsContainer {

    ArgsContainer() {
        args = std::vector<std::string>();
    }

    std::vector<std::string> args;

};

static std::map<__int64, ArgsContainer*> pool = std::map<__int64, ArgsContainer*>();
static __int64 lastIndex = 0;

extern "C" __declspec(dllexport) __int64 CreateArgs()
{
    auto ptr = new ArgsContainer();
    lastIndex++;
    pool.emplace(lastIndex, ptr);
    return lastIndex;
}

extern "C" __declspec(dllexport) void DisposeArgs(__int64 args)
{
    if (pool.count(args) > 0) {
        delete pool[args];
        pool[args] = nullptr;
    }
}

#include <string.h>

extern "C" __declspec(dllexport) void AddToArgs(__int64 args_id, const char* data)
{
    int len = strlen(data);
    char* joker = new char[len + 1];
    memcpy(joker, data, len + 1);
    if (pool.count(args_id) <= 0) {
        printf("[PyBridgeDLL] [ERROR] Stack corrupted\n");
        return;
    }
    auto args = pool[args_id];
    std::string str = std::string(joker);
    args->args.push_back(str);
    delete[] joker;
}

typedef void* (*unknown_func)(...);

static HMODULE cachedMain = 0;
static unknown_func cachedExecute = 0;

extern "C" __declspec(dllexport) void* Execute(const char* function, __int64 args_id) 
{
    if (pool.count(args_id) <= 0) {
        printf("[PyBridgeDLL] [ERROR] Stack corrupted\n");
        return 0;
    }
    if (cachedMain == 0) {
        HMODULE module = GetModuleHandleA("NativeBridgeDLL");
        if (module == 0) {
            printf("[PythonBridgeDLL] [ERROR] Can't connect to NativeBridge!\n");
            return 0;
        }
        unknown_func func = (unknown_func)GetProcAddress(module, "GetMainModule");
        HMODULE mainModule = (HMODULE)func();
        if (mainModule == 0) {
            printf("[PythonBridgeDLL] [ERROR] Can't connect to Game!\n");
            return 0;
        }
        cachedMain = mainModule;
        cachedExecute = (unknown_func) GetProcAddress(mainModule, "ExecuteInGame");
    }
    auto args = pool[args_id];
    //printf("[PyBridgeDLL] [DEBUG] Try to excute function %s with args {%p}\n", function, args);
    cachedExecute(function, args);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OnAttach();
        return TRUE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

