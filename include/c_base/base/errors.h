/*!**********************************************************
 * \file errors.h
 *
 * \brief error handling for the library
 *
 * ### contains:
 * - macros for creating and handling errors
 * - generic result type
 *
 ***********************************************************/
#ifndef ERRORS_H
#define ERRORS_H

#include "base/types.h"
#include "base/strdef.h"
#include "base/macros.h"

/*!
 * \brief unique error identifier
 *
 * to generate a unique error code use the \ref Define_Error()
 *
 * each error should have its unique ErrorCode to easily check
 * which error was used
 */
typedef u64 ErrorCode;
/*!
 * \brief unique error group identifier
 * 
 * to generate unique error group code use the \ref Define_ErrorGroup()
 * 
 * each error group should have its unique code to easily check into which
 * error group does an error belong
 */
typedef u64 ErrorGroup;

/*!
 * \brief error information
 * 
 * contains information of the error with its
 * code the error group it resides in and 
 * the message of the error
 */
typedef struct {
    /*! identifier of the error */
    ErrorCode code;
    /*!  indentifier of the error group */
    ErrorGroup grp;
    /*! message that further describes the error */
    String msg;
} Error;


/*!
 * \brief generates a unique error code
 *
 * the generated error code is a pointer to memory
 * at the __name_str variable
 *
 * the error code can be casted to (String*), the string
 * than contains the name of the error
 * 
 * \code
 * Define_Error(E_ErrorName);
 *
 * String* error_name = (String*) E_ErrorName;
 * \endcode
 *
 * to get the name of the error use the \ref ErrorName() macro
 */
#define Define_Error(name)                               \
    String Concat(__##name, _str) = S(#name);            \
    ErrorCode name = (ErrorCode)&Concat(__##name, _str) 

/*!
 * \brief generates a unique error group code 
 *
 * the generated error group code is a pointer to memory at the
 * __name_str variable
 *
 * the error group code can be casted to (String*), the string
 * than contains the name of the error group
 *
 * \code
 * Define_ErrorGroup(EG_Name);
 *
 * String* error_group_name = (String*) EG_Name;
 * \endcode
 *
 * to get the name of the error use the \ref ErrorGroupName() macro
 */
#define Define_ErrorGroup(name)                          \
    String Concat(__##name, _str) = S(#name);            \
    ErrorGroup name = (ErrorGroup)&Concat(__##name, _str) 


/*!
 * \brief externs an error code
 *
 * macro that externs the specified error code
 *
 * it should be used when defining an error in a header file
 *
 * than the \ref Define_Error() should be used in a c source file
 */
#define Extern_Error(name)      \
    extern ErrorCode name

/*!
 * \brief externs an error group code
 * 
 * macro that externs the specified error group code
 * 
 * it should be used when defining an error group in a header file
 * 
 * than the \ref Define_ErrorGroup() macro should be used in a c source file
 */
#define Extern_ErrorGroup(name) \
    extern ErrorGroup name

/*!
 * \brief shortcut for the error() function
 *
 * expands to a Error struct containing the specified error group, code and message
 */
/* tested */ #define E(errgrp, errcode, errmsg) error((errgrp), (errcode), (errmsg))

/*!
 * \brief returns a string with the error name
 *
 * macro casts the Error.code to a string
 */
/* tested */ 
#define ErrorName(err) (String)(*(String*)err.code)
    
/*!
 * \brief returns a string with the error group name
 * 
 * macro casts the ErrorGroup.code to a string
 */
/* tested */ 
#define ErrorGroupName(err) (String)(*(String*)err.grp)

/*!
 * \brief unspecified error group
 *
 * this error group should only be used when the
 * error does not belong into any other error group
 */
Extern_ErrorGroup(EG_Unspecified);

/*!
 * \brief out of bounds error
 *
 * error for cases where some kind of index is outside of the specified bounds
 */
Extern_Error(E_OutOfBounds);

/*!
 * \brief error for uninitialized variables
 *
 * error should be returned by a function that expects a initialized variable
 * but it gets it unintialized
 */
Extern_Error(E_Uninitialzied);

/*!
 * \brief error for unimplemented features
 *
 * this error can be used when a function or a feature was not yet implemented
 */
Extern_Error(E_Unimplemented);

/*!
 * \brief error for invalid function arguments
 *
 * this error should be used when a function is used
 * with wrong arguments
 */
Extern_Error(E_InvalidArgument);

/*!
 * \brief error for null function arguments
 *
 * this error should be used when a function takes in a null
 * argument
 */
Extern_Error(E_NullArgument);

/*!
 * \brief error for unknown cases
 *
 * dont use this error if there is another error that
 * fits the error better
 */
Extern_Error(E_Unspecified);

/*!
 * \brief out of memory error
 *
 * error that represents a case where memory medium runs out of memory
 *
 * allocators can throw this error
 */
Extern_Error(E_OutOfMemory);

/*!
 * \brief defines a generic result
 *
 * the generated result struct will look like this
 * \code
 * typedef struct {
 *     union {
 *         type value;
 *         Error err;
 *     }
 *     bool ok;
 * } type_Result
 * 
 * \endcode
 *
 * where the type is specified by the user
 *
 * this struct can be used to pass variables from functions
 * that may fail
 *
 * the success of the function can be check trough the result.ok
 * variable if its true the function succeeded and contains a value
 * in result.value
 *
 * if the result.ok is false, the function had failed and the result
 * contains an Error in result.err
 *
 * \param type type of the value stored in the result
 */
#define GenericType_Result(type)                    \
    typedef struct {                                \
        union {                                     \
            type value;                             \
            Error err;                              \
        };                                          \
        bool ok;                                    \
    } Concat(type, _Result);

/*!
 * \brief holds eather a value or error
 *
 * this is a generic type of \ref GenericType_Result()
 *
 * see \ref Generic_TypeResult() to get more information
 */
#define Result(type) Concat(type, _Result)

/*!
 * \brief creates a result with a value
 * 
 * - sets the result.ok to true
 * - sets the result.value
 *
 * \param type type of the result to return
 * \param val value to store to result.value
 *
 * \return result containing the specified value
 */
/* tested */
#define ResultOk(type, val) (Result(type)) {.ok = true, .value = (val)}

/*!
 * \brief creates a result with an error
 *
 * - sets the result.ok to false
 * - sets result.err
 *
 * \param type type of the result to return
 * \param error error to store to result.err
 * 
 * \return result with the specified error
 */
/* tested */ 
#define ResultErr(type, error) (Result(type)) {.ok = false, .err = (error)}

/*!
 * \brief return from a function on error
 *
 * macro that returns a value from the specified expression
 *
 * if the expression returns a result with an error this macro
 * will force the current function to return it too
 *
 * \code
 * 
 * Result(u64) func() {
 *     return ResultOk(u64, 1);
 * }
 *
 * Result(u64) func2() {
 *     return ResultErr(u64, E(...));
 * }
 *
 * Result(s8) test() {
 *     u64 value = TryReturn(s8, u64, func());
 *     // value == 1
 *     u64 value2 = TryReturn(s8, u64, func2());
 *     // test() will now return with the error from func2()
 * }
 *
 * \endcode
 *
 * \param current_func_ret return type of the current function
 * \param expr_ret         return type of the executed expression
 * \param                   expression that returns a Result(expr_ret)
 *
 * \return value of type expr_ret returned by expr
 */
/* tested */
#define TryReturn(current_func_ret, expr_ret, expr) ({\
        Highlight(expr_ret);\
        Result(expr_ret) result = expr;\
        if(!result.ok) {\
            return ResultErr(current_func_ret, result.err);\
        }\
        result.value;\
        })

/*!
 * \brief empty result struct
 *
 * indicates the success of a function
 * if the result.ok equals true the function
 * that returned the struct executed without any problems
 *
 * if the result.ok equals false the function
 * had failed with the error in result.err
 */
typedef struct {
    bool ok;
    Error err;
} Result;

/*!
 * \brief creates an error
 * 
 * \param group   error group of the error
 * \param code    error code of the error
 * \param message error message
 *
 * \return constructed Error struct
 */
/* tested */ static inline Error error(ErrorGroup group, ErrorCode code, String message) {
    return (Error) {.grp = group, .code = code, .msg = message};
}

//#define EmptyResultOk() (Result) {.ok = true}
//#define EmptyResultErr(error) (Result) {.err = (error)} 

// hide this in docs
/*! \cond */
GenericType_Result(none);
GenericType_Result(ptr);

GenericType_Result(u8);
GenericType_Result(u16);
GenericType_Result(u32);
GenericType_Result(u64);

GenericType_Result(s8);
GenericType_Result(s16);
GenericType_Result(s32);
GenericType_Result(s64);

GenericType_Result(b8);
GenericType_Result(b16);
GenericType_Result(b32);
GenericType_Result(b64);
/*! \endcond */

/*!
 * \brief simple typedef for b8_Result
 *
 */
typedef Result(b8) Result(bool);

#endif
