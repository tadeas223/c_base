/*!**********************************************************
 * \file strings.h
 * \brief Length based strings.
 *
 * Structs for length based strings, helper function for creation
 * and manipulation with the strings.
 *
 * \todo Implement character checking functions
************************************************************/
#ifndef STRINGS_H
#define STRINGS_H

#include "base/mem.h"
#include "base/errors.h"
#include "base/types.h"

/*!
 * \brief Macro that converts a c string constant to String8.
 *
 * This macro is used to convert c strings declared with ""
 * to length based strings.
 *
 * \param c_str C-string that will be converted to String8
 * \return Length based string
 */
#define Str8Lit(c_str) str8_cstr_count(c_str, sizeof(c_str) - 1)

/****************************************
 * Structs
****************************************/

/*!
 * \brief String representing 8 bit strings.
 *
 * This struct should be used only for stack allcoated strings.
 * The strings allocated on the heap should use the String8Alloc wrapper struct instead.
 *
 * \param count Length of the string
 * \param Pointer to the memory where the string resides
 *
 * \see String8Alloc
 */
typedef struct {
    u64 count;
    u8 *str;
} String8;

/*!
 * \brief String allocated on the heap.
 *
 * This struct is a wrapper around String8 and should be used only for strings
 * that are not allocated on the stack.
 * This struct does not add any functionality to String8 it's only purpose is for esasier
 * deferentiation of strings allcoated on the stack or heap.
 *
 * \param string The actual string
 */
typedef struct {
    String8 string;
} String8Alloc;

typedef struct String8Node {
    struct String8Node *next;
    struct String8Node *prev;
    String8 string;
} String8Node;

/*!
 * \brief List of strings.
 *
 * This struct represents a doubly linked list that contains String8 strings.
 * It can be used for chaining or splitting strings.
 *
 * \param head Start of the list
 * \param tail End of the list
 * \param node_count Number of Strings sored
 * \param str_count Sum of all characters from the strings inside the list
 */
typedef struct {
    String8Node *head;
    String8Node *tail;
    u64 node_count;
    u64 str_count;
} String8List;

/*!
 * \brief String8 result for error handling.
 * \see errors.h
 */
typedef struct {
    ResultVars(String8);
} String8Result;

/*!
 * \brief String8Alloc result for error handling.
 * \see errors.h
 */
typedef struct {
    ResultVars(String8Alloc);
} String8AllocResult;

/*!
 * \brief String8List result for error handling.
 * \see errors.h
 */
typedef struct {
    ResultVars(String8List);
} String8ListResult;

/****************************************
 * String creation
****************************************/

/*!
 * \brief Creates a string from a pointer to memory and the strings length.
 * 
 * To create a struct allocated in the arena use the str8_alloc() function instead.
 * This function should be used only for strings allocated on the stack.
 *
 * \param str Pointer to the place where strings characters are stored
 * \param count Length of the string
 * 
 * \see str8_alloc()
 */
String8 str8(u8* str, u64 count);

/*!
 * \brief Creates a string from starting poniter and ending pointer.
 * 
 * This function creates a string from a pointer to the start and end of the string.
 *
 * \param start Pointer to the start
 * \param end Pointer to the end
 * \returns New string
 */
String8 str8_range(u8* start, u8* end);

/*!
 * \brief Creates and allocates a string in arena.
 * 
 * Allcoates count memory in the arena and returns string with a pointer to it.
 *
 * \param arena Pointer to the arena in which the memory will be allocated
 * \param count Number of characters inside the string
 * \returns String with a poniter to allocated memory in the arena.
 */
String8Alloc str8_alloc(m_Arena *arena, u64 count);

/*!
 * \brief Creates a String8 from c string and count.
 *
 * \param cstr C-string to be converted into the length based string
 * \param count Length of the string without the null terminator
 * \returns Length based string
 */
String8 str8_cstr_count(const char* cstr, u64 count);

/*!
 * \brief Creates a String8 from c string and calculates its count.
 *
 * This function iterates over each character until the null terminator is found.
 * If you know the c strings length use the str_cstr_count() function instead.
 *
 * \param cstr Pointer to the c string
 * \returns Length based string
 * 
 * \see str_cstr_count()
 */
String8 str8_cstr(const char *cstr);


/****************************************
 * String manipulation
****************************************/

/*!
 * \brief Converts length based strings into c strings
 *
 * This function allocates memory on the arena copies there the contents of the length based string
 * inserts null terminator at the end and returns a pointer to the allocated memory.
 *
 * \param arena Pointer to the arena for allcoations
 * \param string Length based string to convert
 * \return C-string
 */
char* str8_to_cstr(m_Arena *arena, String8 string);

/*!
 * \brief Substrings idk.
 *
 * This function does not allocate anything.
 * The returned string still points to the original.
 * 
 * \param string Original string
 * \param start Index of the first character in the new string
 * \param count Length of the substring 
 *
 * \returns Substring
 */
String8 substr(String8 string, u64 start, u64 count);

/*!
 * \brief Splits the string by a character.
 * 
 * This function splits one string into multiple strings by the specified splitter character.
 * The retuned strings are pointers to the original string. When the original string becomes invalid
 * all of the returned strings will be invalid too.
 *
 * \param arena Arena for temporary allcoations
 * \param string Original string
 * \param splitter Character to split with
 * \returns List of choped strings
 */
String8List str8_split(m_Arena *arena, String8 string, u8 splitter);

/*!
 * \brief Joins multiple strings into one.
 *
 * This function allocates memory in the arena and copies all the strings from the list into it.
 * The function than returns string with a pointer to the arena.
 *
 * \param arena Areana in which the new string will be allocated
 * \param list List of strings to join
 * \returns One joined string
 */
String8Alloc str8_join(m_Arena *arena, String8List *list);

/****************************************
 * String list
****************************************/
/*!
 * \brief Initializes the String8list
 *
 * \param list Pointer to the list to initialize
 */
void str8_list_begin(String8List *list);

/*!
 * \brief Pushes a string to the end of the string list.
 *
 * This function allcoates a new String8Node in the arena and pushes 
 * the node at the end of the string list.
 * 
 * \param arena Pointer to the arena for allocations
 * \param list List in which the string will be pushed
 * \param string String to be pushed
 */
void str8_list_push(m_Arena *arena, String8List *list, String8 string);

/*!
 * \brief Pops the last string from the string list
 *
 * This function simply deletes the pointer to the last node
 * and returns the string stored in it.
 * It does not free the node :(
 *
 * \param list List to pop from
 * \returns The popped string 
 */
String8 str8_list_pop(String8List *list);


/****************************************
 * Simple functions and macros
****************************************/

/*!
 * \brief Returns a character at the specified index from string
 *
 * \pre Pos must be positive and within bounds of the string.
 *
 * \param string String to get characters from
 * \param pos Index of the character
 * \returns Character at pos
 */
u8 str8_at(String8 view, u64 pos);


/****************************************
 * NOT IMPLEMENTED :(
****************************************/
bool is_number(u8 c);
bool is_white_space(u8 c);
bool is_alpha(u8 c);
bool is_alhpanum(u8 c);
bool is_upper(u8 c);
bool is_lower(u8 c);

/*!
 * \brief Expression checking if an ascii character is a number. 
 */
#define IsNumber(c) (c >= '0' && c <= '9')

/*!
 * \brief Expression checking if an ascii character is a space. 
 */
#define IsSpace(c) (c == ' ')

/*!
 * \brief Expression checking if an ascii character is a upper case letter. 
 */
#define IsUpper(c) (c >= 'A' && c <= 'Z')

/*!
 * \brief Expression checking if an ascii character is a lower case letter. 
 */
#define IsLower(c) (c >= 'a' && c <= 'z')

/*!
 * \brief Expression checking if an ascii character is a new line. 
 */
#define IsNewLine(c) (c == '\n')

/*!
 * \brief Expression checking if an ascii character is a letter. 
 */
#define IsAlpha(c) (IsUpper(c) || IsLower(c)))

/*!
 * \brief Expression checking if an ascii character is a letter or number. 
 */
#define IsAphaNum(c) (IsAlpha(c) || IsNumber(c))

#endif
