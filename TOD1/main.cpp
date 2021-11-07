#ifndef _EXE
#include "LogDump.h"
#include "Globals.h"

HMODULE DllModuleHandle;
HANDLE hHookThread = NULL;
HMODULE g_DirectInput = NULL;
DINPUT8CREATEORIGINAL DirectInput8Create_Hooked = NULL;

static FILE* logfile;
void debug(char* message, ...) {
    if (!logfile)
        return;
    SYSTEMTIME t;
    GetLocalTime(&t);
    fprintf(logfile, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    va_list arg;
    va_start(arg, message);
    vfprintf(logfile, message, arg);
    fflush(logfile);
    va_end(arg);
}

extern "C"
{
    HRESULT(*__DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, DWORD riidltf, DWORD ppvOut, DWORD punkOuter);

    __declspec(dllexport) HRESULT _DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, DWORD riidltf, DWORD ppvOut, DWORD punkOuter)
    {
        return __DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
    }
}

void HookDInput()
{
    g_DirectInput = LoadLibrary("C:\\Windows\\System32\\dinput8.dll");
    if (!g_DirectInput) {
        debug("Failed to find dinput8 library!\n");

        return;
    }

    __DirectInput8Create = (HRESULT(*)(HINSTANCE, DWORD, DWORD, DWORD, DWORD))GetProcAddress(g_DirectInput, "DirectInput8Create");

    DirectInput8Create_Hooked = (DINPUT8CREATEORIGINAL)GetProcAddress(g_DirectInput, "DirectInput8Create");

    debug("DirectInput8 hooked! Lib addr: %0.4x, DirectInput8Create method addr: %0.4x\n", &g_DirectInput, &__DirectInput8Create);
}

void PrintVersionInfo()
{
#ifdef INCLUDE_FIXES
    char buf[256] = {};

    debug("******** Kapow! ********\n");
    Utils::GetBuildNumberString(buf);
    strcat(buf, "\n");
    debug(buf);
    Utils::GetEngineAuthor(buf);
    strcat(buf, "\n");
    debug(buf);
    debug("************************\n");
#endif
}

void MemoryHook()
{
    // Insert all hooks here.

    // Redirect all logs into our file.
    hook(0x40C9D0, &LogDump::LogA, PATCH_JUMP);
    hook(0x40C9F0, &LogDump::Print_Impl, PATCH_JUMP);

    // Apply reversed and implemented classes.
    PATCH_WINDOW();
}

void MemoryUnHook()
{
}

//=========================================================================
//
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Begin tracing.
        fopen_s(&logfile, "tod_trace.log", "ab");
        if (!logfile)
            return false;
        debug("Log Started!\n");

        // Replace DirectInput8 methods with stubs.
        PrintVersionInfo();
        HookDInput();

        if (!g_DirectInput)
            return false;

        // Unlock memory.
        register DWORD dwValue;
        if (!VirtualProtect((LPVOID)0x401000, 0x5B1394, PAGE_EXECUTE_READWRITE, &dwValue))
            exit(0);
        else
            debug("_text seg unlocked\n");

        if (!VirtualProtect((LPVOID)0x9B330C, 0x54CF4, PAGE_EXECUTE_READWRITE, &dwValue))
            exit(0);
        else
            debug("_rdata seg unlocked\n");

        if (!VirtualProtect((LPVOID)0xA08000, 0x5a520, PAGE_EXECUTE_READWRITE, &dwValue))
            exit(0);
        else
            debug("_rsrc seg unlocked\n");

        // Replace this dll handle with game's handle.
        DllModuleHandle = hModule;

        // Install memory hooks.
        MemoryHook();

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Free allocated memory.
        MemoryUnHook();

        // Stop tracing.
        if (logfile) {
            debug("Log Closed!\n\n");
            fclose(logfile);
            logfile = NULL;
        }

        // Free DirectInput8 library.
        if (g_DirectInput)
            FreeLibrary(g_DirectInput);

        break;
    }
    return TRUE;
}
#else
#include "stdafx.h"
#include "Window.h"
#endif