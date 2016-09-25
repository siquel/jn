#include "jkn/error.h"
#include <stdio.h> // fprintf
#include <stdarg.h> // va_

namespace jkn
{
    void abort(const char* _file, uint16_t _line, const char* _format, va_list _args)
    {
        __debugbreak();
    }

    void abort(const char* _file, uint16_t _line, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        abort(_file, _line, _format, args);
        va_end(args);
    }
}