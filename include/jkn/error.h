#pragma once

#include <stdint.h>
#include "jkn/macros.h"

namespace jkn
{
    void abort(const char* _file, uint16_t _line, const char* _format, ...);
}

#ifdef JKN_CONFIG_DEBUG
#   define JKN_ASSERT(expr, format, ...)                                                                                              \
            for(;;)                                                                                                                   \
            {                                                                                                                         \
                if (!JKN_IGNORE_C4127(expr))                                                                                          \
                {                                                                                                                     \
                      jkn::abort(__FILE__, uint16_t(__LINE__), "\nAssertion failed: %s\n\t " format "\n", #expr, ##__VA_ARGS__);      \
                }                                                                                                                     \
            break; } 
#else
#   define JKN_ASSERT(condition, ...) for (;;) { break; }
#endif