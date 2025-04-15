/*!**********************************************************
 * \file strdef.h
 *
 * \brief definitions of string types
 *
 * ### contains:
 * - definitions of all string types
 * - macro for string literals
************************************************************/
#ifndef STRDEF_H
#define STRDEF_H

#include "base/types.h"

/*!
 * \brief creates a length based string from a null terminated literal
 *
 * \code
 * String length_based_str = S("c string");
 * \endcode
 */
#define S(cstr) ((String) {.len = sizeof((cstr))-1, .chars = (u8*) (cstr)})

/*!
 * \brief immutable string type 
 *
 * mostly used for storing string literals
 *
 * the contents of the string must never be changed
 * it can lead to segmentation faults
 */
typedef struct {
    /*! number of characters in the string */
    u32 len;
    /*! character array */
    u8* chars;
} String;

/*!
 * \brief muttable string type
 *
 * struct contains just a \ref String struct inside
 *
 * refers to a string that can change its contents without
 * any memory problems
 *
 * the only muttable thing inside the string is the StringMut.str.chars array
 *
 * if you dont need a mutable string use the \ref String
 *
 * \warning the length of the string must not be changed
 */
typedef struct {
    /*! internal string */ 
    String str;
} StringMut;

#endif
