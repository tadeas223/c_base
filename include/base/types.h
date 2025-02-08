/*!**********************************************************
 * \file types.h
 * \brief Primitive type definitions
 *
 * This header file contains primitive definitions of signed,
 * unsigned and bool types along with constants like min, max
 * values of them.
************************************************************/
#ifndef BASE_TYPES_H
#define BASE_TYPES_H

/****************************************
 * signed types
****************************************/
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

/****************************************
 * unsigned types *
****************************************/
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/****************************************
 * boolean types (same as unsigned types)
****************************************/
typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#define bool b8
#define true 1
#define false 0

/****************************************
 * Float types
****************************************/
typedef float f32;
typedef double f64;

/****************************************
 * null types
****************************************/
#define null 0
#define nullptr ((void*) 0)

/****************************************
 * memory macros *
****************************************/

/*!
 * \brief Calculates the number of bytes in x Kilobytes
 */
#define Kilobytes(x) (x * 1024)
/*!
 * \brief Calculates the number of bytes in x Megabytes 
 */
#define Megabytes(x) (x * 1024 * 1024)
/*!
 * \brief Calculates the number of bytes in x Gigabytes 
 */
#define Gigabytes(x) (x * 1024 * 1024 * 1024)

/****************************************
 * min, max values
****************************************/
#define S8_MIN 0x80
#define S16_MIN 0x8000
#define S32_MIN 0x80000000
#define S64_MIN 0x800000000000000

#define S8_MAX 0x7f
#define S16_MAX 0x7fff
#define S32_MAX 0x7fffffff
#define S64_MAX 0x7fffffffffffffff

#define U8_MIN 0x00
#define U16_MIN 0x0000
#define U32_MIN 0x00000000
#define U64_MIN 0x000000000000000

#define U8_MAX 0xff
#define U16_MAX 0xffff
#define U32_MAX 0xffffffff
#define U64_MAX 0xffffffffffffffff

#define B8_MIN 0x00
#define B16_MIN 0x0000
#define B32_MIN 0x00000000
#define B64_MIN 0x000000000000000

#define B8_MAX 0xff
#define B16_MAX 0xffff
#define B32_MAX 0xffffffff
#define B64_MAX 0xffffffffffffffff
#endif
