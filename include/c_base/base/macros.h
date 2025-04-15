/*!**********************************************************
 * \file macros.h
 *
 * \brief simple macros for the library
 *
 * macros for the macro magic this library is using
************************************************************/
#ifndef MACROS_H
#define MACROS_H

/*!
 * \brief concats two expressions together
 *
 * this macro is useful for naming functions inside
 * other macros for generics
 *
 * \code
 * Concat(hello, _world); // expands to hello_world
 * \endcode
 *
 * \param a first expression
 * \param b second expression
 *
 * \return ab
 */
#define Concat(a, b) a##b

/*!
 * \brief highlights the input
 *
 * is used only for prittier code with a highlighter
 *
 * simply makes the input registeres as used and therfore the 
 * highlighter gives a color to tye input
 *
 * the input need to be a type
 *
 * \param type type to highlight
 */
#define Highlight(type) (void)sizeof(type)

#endif
