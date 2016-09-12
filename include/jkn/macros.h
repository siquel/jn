#pragma once

#define JKN_STRINGIZE_(x) #x
#define JKN_STRINGIZE(x) JKN_STRINGIZE_(x)

#define JKN_CONCATENATE(x, y) x ## y
#define JKN_CONCATENATE(x, y) JKN_CONCATENATE_(x, y)