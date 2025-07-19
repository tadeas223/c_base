#ifndef ENV_H
#define ENV_H

#if defined(__GNUC__)
  #define COMPILER_GCC
#elif defined(__clang__)
  #define COMPILER_CLANG
#else
  #warning "env.h -> unknown compiler"
#endif

#if !defined(__STDC_VERSION__)
  #define STD_C89
#elif __STDC_VERSION__ < 199409L
  #define STD_C89
#elif __STDC_VERSION__ < 199901L
  #define STD_C95
#elif __STDC_VERSION__ < 201112L
  #define STD_C99
#elif __STDC_VERSION__ < 201710L
  #define STD_C11
#elif __STDC_VERSION__ < 202000L
  #define STD_C17
#else
  #define STD_C2X
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
  #define PLATFORM_MACOS
#elif defined(__linux__)
  #define PLATFORM_LINUX
#elif defined(__unix__)
  #define PLATFORM_UNIX
#elif defined(__ANDROID__)
  #define PLATFORM_ANDROID
#elif defined(__EMSCRIPTEN__)
  #define PLATFORM_WEB
#else
  #warning "env.h -> unknown platform"
#endif

#if (defined(COMPILER_GCC) || defined(COMPILER_CLANG)) && defined(OPT_COMPILER_FEATURES)
  #define ATTR_Cleanup(func) __attribute__((cleanup(func)))
#endif

#endif
