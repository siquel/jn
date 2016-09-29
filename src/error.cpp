#include "jkn/error.h"
#include <stdio.h> // fprintf
#include <stdarg.h> // va_
#include <alloca.h> // alloca
#include <string.h> // memcpy
#include "jkn/debug.h" // debugOutput

namespace jkn
{
#if JKN_COMPILER_MSVC && JKN_COMPILER_MSVC < 1800 // vs2013
#   define va_copy(a, b) (a) = (b)
#endif

    // Returns the number of characters that would have been written if _bufSize had been sufficiently large, not counting the terminating null character.
    int32_t vsnprintf(char* _buffer, size_t _bufSize, const char* _format, va_list _args)
    {
#if JKN_PLATFORM_WINDOWS
        // copy vargs, because vsnprintf_s consumes the arg list
        va_list copyAp;
        va_copy(copyAp, _args);
        // if the string doesn't fit vsnprintf_s will return -1
        int32_t len = ::vsnprintf_s(_buffer, _bufSize, size_t(-1), /* MAX */ _format, copyAp);
        va_end(copyAp); 

        // If the return val is -1 use _vscprintf to figure out how big the buffer should be
        return -1 != len ? len : ::_vscprintf(_format, _args);
#elif JKN_PLATFORM_LINUX
        return ::vsnprintf(_buffer, _bufSize, _format, _args);
#endif
    }
    
    // Return number of characters that would have been written if n had been sufficiently large, not counting the terminating null character.
    int32_t snprintf(char* _buf, size_t _bufSize, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        int32_t len = jkn::vsnprintf(_buf, _bufSize, _format, args);
        va_end(args);
        return len;
    }

    void abort(const char* _file, uint16_t _line, const char* _format, va_list _args)
    {
        char buffer[512];
        char* out = buffer;
        
        va_list copyAp;
        va_copy(copyAp, _args);

        int32_t len = jkn::snprintf(buffer, sizeof(buffer), "%s:(%d) ", _file, _line);
        int32_t total = len + jkn::vsnprintf(out + len, sizeof(buffer) - len, _format, copyAp);

        va_end(copyAp);

        // not enough room 
        if ((int32_t)sizeof(buffer) < total)
        {
            out = (char*)alloca(total + 1);
            memcpy(out, buffer, len);
            jkn::vsnprintf(out + len, total - len, _format, _args);
        }
        out[total] = '\0';
        jkn::debugOutput(buffer);
        jkn::debugBreak();
    }

    void abort(const char* _file, uint16_t _line, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        abort(_file, _line, _format, args);
        va_end(args);
    }
}
