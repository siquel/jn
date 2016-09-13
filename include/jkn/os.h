#pragma once

#include "platform.h"

#if JKN_PLATFORM_LINUX
#   include <unistd.h> // usleep
#endif

namespace jkn
{
    inline void sleep(uint32_t _milliseconds)
    {
#if JKN_PLATFORM_WINDOWS
        ::Sleep(_milliseconds);
#elif JKN_PLATFORM_LINUX
        usleep(_milliseconds * 1000);
#endif
    }
}
