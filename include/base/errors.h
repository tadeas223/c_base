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
#define ResultVars(type) type value; u32 err; bool ok

/*!
 * \brief Same as ResultVars() but for empty Results
 */
#define EmptyResultVars() u32 err; bool ok

/*!
 * \brief Macro that crates a successfull Result
 * 
 * This macro sets these variables of the Result:
 *  - ok = true
 *  - value = val
 *  - err = 0
 *
 * Example of this macro:
 * \code
 * IntResult func() {
 *     return (IntResult) ResultOK(1) // returns a IntResult containing a int value 1
 * }
 * \endcode
 *  
 * \param val value that should be returned on successfull function call
 */
#define ResultOK(val) {.ok = true, .value = val, .err = 0 }

/*!
 * \brief Macro that crates a unsuccessful Result
 *
 * This macro sets these variables of the Result:
 *  - ok = false 
 *  - value = null
 *  - err = code
 *  
 * Example of this macro:
 * \code
 * IntResult func() {
 *     return (IntResult) ResultERR(1) // returns a IntResult with an error code 1
 * }
 * \endcode
 *
 * \param code error code, indicating a unsuccessful state
 */
#define ResultERR(code) {.ok = false, .err = code }

/*!
 * \brief Same as ResultOK() but for Results without an value.
 */
#define EmptyResultOK() {.ok = true, .err = 0 }
/*!
 * \brief Same as ResultERR() but for Results without an value.
 */
#define EmptyResultERR(code) {.ok = false, .err = code }

/*!
 * \brief Global error codes.
 * \warning These codes should not overlap with any other error codes.
 */
typedef enum { 
    ERR_UNSPECIFIED = U32_MAX,
} ErrorCode;

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
