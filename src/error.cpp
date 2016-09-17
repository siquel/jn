#include "jkn/error.h"
#include <stdio.h> // fprintf
namespace jkn
{
    void abort(const char* _file, uint16_t _line, const char* _format, ...)
    {
        fprintf(stderr, "Hello!");
    }
}