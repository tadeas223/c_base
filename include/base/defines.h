#ifndef DEFINES_H
#define DEFINES_H

#include "base/strings.h"

/* comiler */
# if defined(__clang__)
#   define COMPILER_CLANG 1
# elif defined(__GNUC__)
#   define COMPILER_GCC 1
# else 
#   error unknown compiler
# endif

/* platform */
# if defined(__linux__)
#   define PLATFORM_LINUX 1
# elif defined(__APPLE__)
#   define PLATFORM_MACOS 1
# elif defined(__WINDOWS__)
#   define PLATFORM_WINDOWS 1
# else
#   error unkown platform 
# endif

# if defined(PLATFORM_WINDOWS)
#   define NEW_LINE Str8Lit("\n\r")
# else
#   define NEW_LINE Str8Lit("\n") 
# endif

/* handy macros */
#define Stmt(code) do { code } while(0)
#endif 
