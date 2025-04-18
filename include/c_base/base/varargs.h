#ifndef VARARGS_H
#define VARARGS_H

#include "base/types.h"
#include "stdarg.h"

#define __VarargsLen(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,\
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,\
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,\
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,\
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...) N

#define VarargsLen(args...) __VarargsLen(args,\
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41,\
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31,\
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21,\
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11,\
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define VA(args...) VarargsLen(args), args

#define Varargs u32 varargs_len, ...

#define Args varargs_len, args 

#define Varlist u32 varargs_len, va_list args

#define VarargsStart() va_list args; va_start(args, varargs_len)

#define VarargsEnd() va_end(args)

#define VarargsBlock(code...)\
    VarargsStart();\
    do code while(0);\
    VarargsEnd();\

#define VarlistForeach(type, code...)                     \
    do {                                                  \
        va_list args_cpy;                                 \
        va_copy(args_cpy, args);                          \
        for(u32 iter = 0; iter < varargs_len; iter++) {   \
            type value = va_arg(args_cpy, type);          \
            do code while(0);                             \
        }                                                 \
        va_end(args_cpy);                                 \
    } while(0)

#define VarargsForeach(type, code...)                     \
    do {                                                  \
        va_list args_cpy;                                 \
        va_copy(args_cpy, args);                          \
        for(u32 iter = 0; iter < varargs_len; iter++) {   \
            type value = va_arg(args_cpy, type);          \
            do code while(0);                             \
        }                                                 \
        va_end(args_cpy);                                 \
    } while(0)

#endif
