#ifndef DEFINES_H
#define DEFINES_H

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

#endif 
