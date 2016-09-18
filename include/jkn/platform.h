#pragma once

#define JKN_COMPILER_MSVC             0
#define JKN_COMPILER_CLANG            0
#define JKN_COMPILER_GCC              0

#define JKN_PLATFORM_WINDOWS          0
#define JKN_PLATFORM_LINUX            0

#define JKN_ARCH_32BIT                0
#define JKN_ARCH_64BIT                0

#define JKN_CPU_LITTLE_ENDIAN         0
#define JKN_CPU_BIG_ENDIAN            0

#define JKN_CPU_x86                   0
#define JKN_CPU_PPC                   0

#if defined(__clang__)
#   undef JKN_COMPILER_CLANG
#   define JKN_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(_MSC_VER)
#   undef JKN_COMPILER_MSVC
#   define JKN_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#   undef JKN_COMPILER_GCC
#   define JKN_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#   error "Unsupported compiler"
#endif

#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#   undef JKN_ARCH_64BIT
#   define JKN_ARCH_64BIT 1
#else
#   undef JKN_ARCH_32BIT
#   define JKN_ARCH_32BIT 1
#endif

#if defined(_M_PPC)        || \
	defined(__powerpc__)   || \
    defined(__powerpc64__)
#   undef JKN_CPU_PPC
#   define JKN_CPU_PPC 1
#   define JKN_CACHE_LINE_SIZE 128
#elif defined(_M_IX86)    || \
	  defined(_M_X64)     || \
	  defined(__i386__)   || \
      defined(__x86_64__)
#   undef JKN_CPU_X86
#   define JKN_CPU_X86  1
#   define JKN_CACHE_LINE_SIZE 64
#endif

#if defined(_WIN32) || defined(_WIN64)
#     undef JKN_PLATFORM_WINDOWS
#     ifndef NOMINMAX
#         define NOMINMAX
#     endif
#     if !defined(WINVER) && !defined(_WIN32_WINNT) 
#         define WINWER 0x0601 // win 7
#         define _WIN32_WINNT 0x0601
#     endif
#     define JKN_PLATFORM_WINDOWS _WIN32_WINNT
#elif defined(__linux__)
#     undef JKN_PLATFORM_LINUX
#     define JKN_PLATFORM_LINUX       1
#endif

#define JKN_PLATFORM_POSIX (0 \
                          || JKN_PLATFORM_LINUX \
                           )


#if JKN_PLATFORM_PPC
#   undef JKN_CPU_BIG_ENDIAN
#   define JKN_CPU_BIG_ENDIAN 1
#else
#   undef JKN_CPU_LITTLE_ENDIAN
#   define JKN_CPU_LITTLE_ENDIAN 1
#endif

#if JKN_ARCH_32BIT
#   define JKN_ARCH_NAME  "32-bit"
#else
#   define JKN_ARCH_NAME  "64-bit"
#endif

#if JKN_PLATFORM_WINDOWS
#   define JKN_PLATFORM_NAME      "Windows"
#elif JKN_PLATFORM_LINUX
#   define JKN_PLATFORM_NAME      "Linux"
#endif

#if JKN_COMPILER_MSVC
#   if JKN_COMPILER_MSVC >= 1900 // vs2015
#       define JKN_COMPILER_NAME  "MSVC 14.0"
#   elif JKN_COMPILER_MSVC >= 1800 // vs2013
#       define JKN_COMPILER_NAME  "MSVC 12.0"
#   else
#       define JKN_COMPILER_NAME  "MSVC"
#   endif
#elif JKN_COMPILER_CLANG
#   define JKN_COMPILER_NAME      "Clang " \
                                    JKN_STRINGIZE(__clang_major__) "." \
                                    JKN_STRINGIZE(__clang_minor__) "." \
                                    JKN_STRINGIZE(__clang_patchlevel__)
#elif JKN_COMPILER_GCC
#   define JKN_COMPILER_NAME      "GCC " \
                                    JKN_STRINGIZE(__GNUC__) "."       \
                                    JKN_STRINGIZE(__GNUC_MINOR__) "." \
                                    JKN_STRINGIZE(__GNUC_PATCHLEVEL__) 
#endif
