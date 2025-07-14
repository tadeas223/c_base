#ifndef TYPES_H
#define TYPES_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef signed char  s8;
typedef signed short s16;
typedef signed int   s32;
typedef signed long  s64;

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

typedef float f32;
typedef double f64;

#define null 0
#define true 1
#define false 0
#define bool b8

typedef void* ptr;

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
