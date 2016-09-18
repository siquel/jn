#pragma once

#include "platform.h"
#if JKN_PLATFORM_WINDOWS
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif

namespace jkn
{
    inline void debugBreak()
    {
#if JKN_PLATFORM_WINDOWS
        __debugbreak();
#endif
    }

    inline void debugOutput(const char* _out)
    {
#if JKN_PLATFORM_WINDOWS
        ::OutputDebugStringA(_out);
#endif
    }
}

