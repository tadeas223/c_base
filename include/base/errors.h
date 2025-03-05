/*!**********************************************************
 * \file errors.h
 * \brief Error handling for the library.
 *
 * If a function possibly returns an error it's return
 * type should be some kind of Result.
 * 
 * The result can be than checked for errors and it's value
 * can be safely used (see the example bellow).
 * 
 * \code
 * IntResult r_val = func_that_may_error();
 * if(!r_val.ok) {
 *     // handle the error
 * }
 * // use the returned value
 * r_val.value += 10;
 * \endcode
 *
 * This function also defines Result types for primitive types
 * like u8, u16, s8 etc...
************************************************************/
#ifndef ERRORS_H
#define ERRORS_H

#include "base/types.h"
#include <sys/types.h>

/*!
 * \brief error type.
 */
typedef u64 Error;

/*!
 * \brief Creates a error variable.
 * 
 * Use this macro in a .c file to avoid multiple definitions.
 * To add the error in a header file use the ExternError() macro.
 *
 * The variable that is crated is a plain u8 number.
 * The pointer of this variable can be used to distinguish errors.
 * 
 * \param err_name Name of the error
 */
#define DefineError(err_name) u8 val_##err_name = 0x55; Error err_name = (u64) &val_##err_name

/*!
 * \brief Defines an error in a header file.
 * 
 * This macro should be used to define an error in a header file.
 * The error should be than define with the DefineError() macro in the 
 * coresponding .c file.
 *
 * \param err_name Name of the error
 */
#define ExternError(err_name) extern Error err_name;

/*!
 * \brief Converts the error into a string
 */
#define ErrorStr(err_name) Str8Lit(#err_name)

/****************************************
 * Generic error types
****************************************/

/*!
 * \brief Default error for everything
 *
 * It is adviced to use more specific messages than this.
 *
 * \see DefineError()
 */
ExternError(UnspecifiedError);


/*!
 * \brief Error returned when a unhandled null address appeares. 
 */
ExternError(NullPoniterError);

/*!
 * \brief Error for int overflows
 *
 * This eror should be returned when an overflow happens.
 */
ExternError(OverflowError);

/*!
 * \brief Error for indication invalid arguments passed in functions.
 */
ExternError(InvalidArgumentError);

/*!
 * \brief Macro for betting a result value without checking
 *
 * This macro just appends .value.
 * The purpose of this is to make getting result values easyer.
 * 
 * \code
 * int value = func_returning_result()f;
 * \endcode
 */
#define f .value

/*!
 * \brief Checks an error type
 *
 * An expression that checks if the err_name and err are equal.
 *
 * \param err_name Name of the error to check.
 * \param err error to check
 * \returns true if the err is error of the err_name type, false otherwise
 */
#define IsError(err_name, err) ((err_name) == (err))

/*!
 * \brief Macro for easier creation of the Result structs.
 *
 * Example of this macro:
 * \code
 * struct IntResult {
 *     ResultVars(int);
 *     // ...
 *     // Optionaly more variables here
 *     // ...
 * };
 * \endcode
 */
#define ResultVars(type) type value; Error err; bool ok

/*!
 * \brief Same as ResultVars() but for empty Results
 */
#define EmptyResultVars() Error err; bool ok

/*!
 * \brief Macro that crates a successfull Result
 * 
 * This macro sets these variables of the Result:
 *  - ok = true
 *  - value = val
 *  - err = null
 *
 * Example of this macro:
 * \code
 * U8Result func() {
 *     return (U8Result) ResultOK(1) // returns a IntResult containing a int value 1
 * }
 * \endcode
 *  
 * \param val value that should be returned on successfull function call
 */
#define ResultOK(val) {.ok = true, .value = (val), .err = null }

/*!
 * \brief Macro that crates a unsuccessful Result
 *
 * This macro sets these variables of the Result:
 *  - ok = false 
 *  - value = null
 *  - err = &err_name
 *  
 * Example of this macro:
 * \code
 * U8Result func() {
 *     return (U8Result) ResultERR(ERR_UNSPECIFIED) // returns a U8Result with an unspecified error
 * }
 * \endcode
 *
 * \param err_name name of the error, indicating a unsuccessful state
 */
#define ResultERR(err_name) {.ok = false, .err = err_name}

/*!
 * \brief Same as ResultOK() but for Results without an value.
 */
#define EmptyResultOK() {.ok = true, .err = null }
/*!
 * \brief Same as ResultERR() but for Results without an value.
 */
#define EmptyResultERR(err_name) {.ok = false, .err = err_name}

/*!
 * \brief Result that does not contain a value.
 * 
 * All functions void functions that may error should return this struct.
 */
typedef struct {
    EmptyResultVars();
} Result;


/****************************************
 * Unsigned Result structs
****************************************/
typedef struct {
    ResultVars(u8);
} U8Result;

typedef struct {
    ResultVars(u16);
} U16Result;

typedef struct {
    ResultVars(u32);
} U32Result;

typedef struct {
    ResultVars(u64);
} U64Result;


/****************************************
 * Signed Result structs
****************************************/
typedef struct {
    ResultVars(s8);
} S8Result;

typedef struct {
    ResultVars(s16);
} S16Result;

typedef struct {
    ResultVars(s32);
} S32Result;

typedef struct {
    ResultVars(s64);
} S64Result;


/****************************************
 * Bool Result structs
****************************************/
typedef struct {
    ResultVars(b8);
} B8Result;

typedef struct {
    ResultVars(b16);
} B16Result;

typedef struct {
    ResultVars(b32);
} B32Result;

typedef struct {
    ResultVars(b64);
} B64Result;

#define BoolResult B8Result

/****************************************
 * Float Result structs
****************************************/

typedef struct {
    ResultVars(f32);
} F32Result;

typedef struct {
    ResultVars(f64);
} F64Result;

/****************************************
 * C type Results
****************************************/

typedef struct { 
    ResultVars(void*);
} VoidPtrResult;

#endif
