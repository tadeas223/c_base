/*!**********************************************************
 * \file defines.h
 * \brief Macros defining the compilation enviroment.
 *
 * Contains definition of the Platform that the program will
 * be compiled on and the compiler that is used.
 *
 * Every definition that starts with **PLATFORM_** refers to the 
 * platform (operating system) that the code is compiled for
 * 
 * every definition that starts with **COMPILER_** refers to the 
 * compiler that is used to compile the code.
 * 
 * \todo add more platforms and compiler macros
************************************************************/
#ifndef DEFINES_H
#define DEFINES_H

/* comiler */
# if defined(__clang__)
#   define COMPILER_CLANG 1
# elif defined(__GNUC__)
#   define COMPILER_GCC 1
# else 
#   error unknown compiler
# endif

/* platform */
# if defined(__linux__)
#   define PLATFORM_LINUX 1
# elif defined(__APPLE__)
#   define PLATFORM_MACOS 1
# elif defined(__WINDOWS__)
#   define PLATFORM_WINDOWS 1
# else
#   error unkown platform 
# endif

/****************************************
 * Handy macros
****************************************/

/*!
 * @brief Simple macro that wraps its contents into a do {} while(0) loop
 */
#define Stmt(code) do { code } while(0)
#endif 
