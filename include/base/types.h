#ifndef TYPES_H
#define TYPES_H

#include "base/macros.h"

#define P(type) Concat(type, _p)
#define P2(type) Concat(type, _pp)
#define P3(type) Concat(type, _ppp)

#define Type(name, def...)             \
    typedef def name;                  \
    typedef name* Concat(name, _p);    \
    typedef name** Concat(name, _pp);  \
    typedef name*** Concat(name, _ppp)

#define TypeStruct(name, def...)        \
    typedef struct name name;           \
    typedef name* Concat(name, _p);     \
    typedef name** Concat(name, _pp);   \
    typedef name*** Concat(name, _ppp); \
    struct name def

#define TypeStructNoDecl(name, def...) \
    typedef def name;                  \
    typedef name* Concat(name, _p);    \
    typedef name** Concat(name, _pp);  \
    typedef name*** Concat(name, _ppp)

/****************************************
 * Unsigned integer types
****************************************/

Type(u8,  unsigned char);
Type(u16, unsigned short);
Type(u32, unsigned int);
Type(u64, unsigned long long);

/****************************************
 * Signed integer types
****************************************/

Type(s8,  signed char);
Type(s16, signed short);
Type(s32, signed int);
Type(s64, signed long long);


/****************************************
 * Boolean types (same as unsigned ints)
****************************************/

Type(b8,  u8);
Type(b16, u16);
Type(b32, u32);
Type(b64, u64);

Type(bool, b8);

#define true 1
#define false 0

/****************************************
 * Float types
****************************************/

Type(f32, float);
Type(f64, double);

/****************************************
 * Null types
****************************************/

typedef b8 none;
#define null 0

/****************************************
 * Void :(
****************************************/

typedef void* void_p;
typedef void* ptr;
typedef void** void_pp;
typedef void*** void_ppp;

/****************************************
 * Memory macros
****************************************/

#define Kilobytes(x) (x * 1024)
#define Megabytes(x) (x * 1024 * 1024)
#define Gigabytes(x) (x * 1024 * 1024 * 1024)

/****************************************
 * Constants
****************************************/

#define Max(type) Concat(type, _MAX)
#define Min(type) Concat(type, _MIN)

#define u8_MIN  (u8)  0
#define u16_MIN (u16) 0
#define u32_MIN (u32) 0
#define u64_MIN (u64) 0

#define u8_MAX  (u8)  0xff
#define u16_MAX (u16) 0xffff
#define u32_MAX (u32) 0xffffffff
#define u64_MAX (u64) 0xffffffffffffffff

#define s8_MIN  (s8)  0x80
#define s16_MIN (s16) 0x8000
#define s32_MIN (s32) 0x80000000
#define s64_MIN (s64) 0x8000000000000000

#define s8_MAX  (s8)  0x7f
#define s16_MAX (s16) 0x7fff
#define s32_MAX (s32) 0x7fffffff
#define s64_MAX (s64) 0x7fffffffffffffff

#define f32_MIN (f32) 0xff7fffff
#define f64_MIN (f64) 0xffefffffffffffff

#define f32_MAX (f32) 0x7f7fffff
#define f64_MAX (f64) 0x7fefffffffffffff

#endif
