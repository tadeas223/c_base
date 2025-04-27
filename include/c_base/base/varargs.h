/*!**********************************************************
 * \file varargs.h
 * \brief varargs abstraction for the library
 *
 * contains abstractions over the normal va_list arguments
 * from C.
 *
 * this varargs implementation also carryes the number of arguments
 * put in.
************************************************************/
#ifndef VARARGS_H
#define VARARGS_H

#include "base/types.h"
#include "stdarg.h"

/*!
 * \brief intermidiate macro for counting variable arguments
 *
 * see also: \ref VarargsLen()
 */
#define __VarargsLen(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,\
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,\
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,\
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,\
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...) N

/*!
 * \brief returns the number of args specified
 *
 * counts the arguments up to 50 than i breaks
 *
 * \warning do not use for more than 50 arguments
 */
#define VarargsLen(args...) __VarargsLen(args,\
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41,\
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31,\
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21,\
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11,\
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/*!
 * \brief vararg input for the Varargs macro
 *
 * inserts number of arguments in front.
 * is compatable with Varargs function input.
 *
 * \code
 * void test_func(Varargs) {
 *
 * }
 *
 * int main() {
 *     test_func(VA(1,2,3,4,5));
 * }
 *
 * \endcode
 */
#define VA(args...) VarargsLen(args), args

/*!
 * \brief variable input for a function
 *
 * if a function has Varargs the function has variable ammount of
 * input
 *
 * \code
 * void test_func(Varargs) {
 *
 * }
 * \endcode
 */
#define Varargs u32 varargs_len, ...

/*!
 * \brief variable holding Varargs
 *
 * this variable can be used only in the VarargsBlock or
 * between VarargsStart and VarargsEnd
 *
 * this macro should be used to pass input arguments to a function
 * having a Varlist as a input argument
 */
#define Args varargs_len, args 

/*!
 * \brief variable input for a function that should be called in another variable input function
 *
 * same as a function that takes in va_list as an argument
 *
 * \code
 * void test_func_varlist(Varlist) {
 *     // do stuff with the arguments 
 * }
 * void test_func(Varargs) {
 *     VarargsStart();
 *     test_func_varlist(Args);
 *     VarargsEnd();
 * }
 * \endcode
 */
#define Varlist u32 varargs_len, va_list args

/*!
 * \brief indicates the start of the Varargs
 *
 * initializes the needed variables for work with variable arguments
 */
#define VarargsStart() va_list args; va_start(args, varargs_len)

/*!
 * \brief indicates the end of the Varargs
 *
 * destroys all the variable created by VarargsStart()
 */
#define VarargsEnd() va_end(args)

/*!
 * \brief block of code in which the Varargs are valid
 *
 * calls VarargsStart, than code, than VarargsEnd
 *
 * \param code code to be executed within the scope of Varargs
 */
#define VarargsBlock(code...)\
    VarargsStart();\
    do code while(0);\
    VarargsEnd();\

/*!
 * \brief loops trough all variables stored in a Varlist 
 *
 * can be only used in a function that takes in Varlist as a parameter
 * 
 * the foreach exposes the variables iter (the index of ineration) and
 * value (current variable taht is processed)
 *
 * \param type type of the variable that the Varlist contaions
 * \param code code to be executed for each variable
 *
 */
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

/*!
 * \brief loops trough all variables stored in a Varargs 
 *
 * can be only used in a function that takes in Varargs as a parameter
 * 
 * the foreach exposes the variables iter (the index of ineration) and
 * value (current variable taht is processed)
 *
 * this macro can be only called on VarargsBlock or between VarargsStart and VarargsEnd
 * 
 * \param type type of the variable that the Varargs contaions
 * \param code code to be executed for each variable
 *
 */
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
