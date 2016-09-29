#pragma once

#include "platform.h"
#if JKN_PLATFORM_WINDOWS
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#elif JKN_PLATFORM_LINUX
#   include <stdio.h> // fputs, fflush
#endif

namespace jkn
{
    inline void debugBreak()
    {
#if JKN_PLATFORM_WINDOWS
        __debugbreak();
#elif JKN_PLATFORM_LINUX
        __asm__("int $3");
#endif
    }

    inline void debugOutput(const char* _out)
    {
#if JKN_PLATFORM_WINDOWS
        ::OutputDebugStringA(_out);
#elif JKN_PLATFORM_LINUX
        fputs(_out, stdout);
        fflush(stdout);
#endif
    }
}

