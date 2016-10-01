#pragma once

#define JKN_STRINGIZE_(x) #x
#define JKN_STRINGIZE(x) JKN_STRINGIZE_(x)

#define JKN_CONCATENATE_(x, y) x ## y
#define JKN_CONCATENATE(x, y) JKN_CONCATENATE_(x, y)

namespace jkn
{
    // Conditional expression is constant
    inline bool ignoreC4127(bool _x) 
    { 
        return _x; 
    }
}

#define JKN_IGNORE_C4127(x) jkn::ignoreC4127(!!(x))