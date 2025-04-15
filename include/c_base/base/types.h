/*!**********************************************************
 * \file types.h
 *
 * \brief types and useful constants for the library
 *
 * ### types include:
 * - unsigned types        (uxx)
 * - signed types          (sxx)
 * - boolean or flag types (bxx)
 * - float types           (fxx)
 *
 * also contains constants like min and max values of the types
 *
 * for more information check the source
 * code of this header file
************************************************************/
#ifndef TYPES_H
#define TYPES_H

#include "base/macros.h"

/****************************************
 * unsigned integer types
****************************************/

/*! \cond */
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
/*! \endcond */

/****************************************
 * signed integer types
****************************************/

/*! \cond */
typedef signed char      s8;
typedef signed short     s16;
typedef signed int       s32;
typedef signed long long s64;
/*! \endcond */

/****************************************
 * boolean types (same as unsigned ints)
****************************************/

/*! \cond */
typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;
/*! \endcond */

/*!
 * \brief use this type for true/false values
 */
typedef b8 bool;

/* \cond */
#define true 1
#define false 0
/* \endcond */

/****************************************
 * float types
****************************************/

/*! \cond */
typedef float  f32;
typedef double f64;
/*! \endcond */

/****************************************
 * null types
****************************************/

/*! \cond */
typedef b8 none;
#define null 0
/*! \endcond */

/****************************************
 * void :(
****************************************/

/*! 
 * \brief value for a generic pointer
 *
 * this value should be used in any generic type
 * that contains a pointer
 *
 * each pointer is the same size so there is no need
 * to make each generic have a variant for each pointer type
 */
typedef void* ptr;

/****************************************
 * memory macros
****************************************/

/*!
 * \brief converts kilobytes (input) into bytes (output)
 *
 * be aware of possible overflows when using with big values
 *
 * \param x number of kilobytes 
 * 
 * \returns bytes
*/
#define Kilobytes(x) (x * 1024)

/*!
 * \brief converts megabytes (input) into bytes (output)
 *
 * be aware of possible overflows when using with big values
 *
 * \param x number of megabytes
 * 
 * \returns bytes
 */
#define Megabytes(x) (x * 1024 * 1024)

/*!
 * \brief converts gigabytes (input) into bytes (output)
 *
 * be aware of possible overflows when using with big values
 *
 * \param x number of gigabytes 
 * 
 * \returns bytes
 */
#define Gigabytes(x) (x * 1024 * 1024 * 1024)

/****************************************
 * constants
****************************************/

/*!
 * \brief generic way to get a max value of a type
 *
 * this is helpful for writing generic macros that need
 * maximum value of a unknown type
 *
 * to make your max value compatible with this macro follow
 * the naming convention below 
 * 
 * \code
 * type_MAX
 * \endcode
 */
#define Max(type) Concat(type, _MAX)

/*!
 * \brief generic way to get a min value of a type
 *
 * this is helpful for writing generic macros that need
 * minimum value of a unknown type
 *
 * to make your min value compatible with this macro follow
 * the naming convention below 
 * 
 * \code
 * type_MIN
 * \endcode
 */
#define Min(type) Concat(type, _MIN)

/*! \cond */
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
/*! \endcond */

#endif
