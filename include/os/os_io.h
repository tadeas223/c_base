/*!**********************************************************
 * \file os_io.h
 * \brief IO operations that depend on os
************************************************************/
#ifndef OS_IO_H
#define OS_IO_H

#include "base/mem.h"
#include "base/strings.h"
#include "base/errors.h"


/****************************************
 * Console write macros
****************************************/

/*!
 * \brief Wrapper around os_console_write() function.
 * 
 * \see os_console_write()
 */
#define ConsoleWrite(string) os_console_write(string);
/*!
 * \brief Macro that writes to console and inserts new line at the end.
 * 
 * This macro calls os_console_write() funtion for the string and again for a new line character
 * \see os_console_write()
 */
#define ConsoleWriteLn(string) os_console_write(string); os_console_write(Str8Lit("\n"));


/****************************************
 * Console read macros
****************************************/
/*!
 * \brief Reads from console until a new line character.
 */
#define ConsoleReadLn(arena) os_console_read_until(arena, '\n')
/*!
 * \brief Reads from console until a the specified character
 */
#define ConsoleReadUntil(arena, splitter) os_console_read_until(arena, splitter)


/****************************************
 * Structs
****************************************/

/*!
 * \brief Opaque struct representing a file.
 *
 * This structs implementation depends on the operation system
 */
typedef struct File File;

/*!
 * \brief File result for error handling.
 */
typedef struct {
    ResultVars(File*);
} FileResult;

/*!
 * \brief File access enums.
 * 
 * W = Write
 * R = Read
 * A = Append
 */
typedef enum {
    FILE_W,
    FILE_R,
    FILE_RW,
    FILE_A, /* append mode */
    FILE_AR,
} FileMode;

/****************************************
 * Files
****************************************/

/*!
 * \brief Checks if a file exists.
 *
 * \warning This function returns false on every error even if the file exists.
 * 
 * \param arena Arena for temporary allocations
 * \param path Path to check
 * \returns True of the file exists, false otherwise
 */
bool os_file_exists(m_Arena *arena, String8 path);

/*!
 * \brief Opens a file.
 *
 * This function opens a file with the specified access mode and
 * returns a handle to it.
 *
 * \param arena Pointer to arena to allocate the file handle.
 * \param path Path to the file
 * \param mode Access mode
 * \returns Handle to the file or error
 */
FileResult os_file_open(m_Arena *arena, String8 path, FileMode mode);

/*!
 * \brief Creates or erases a file.
 *
 * Finction that creates a file if it does not exist.
 * If the file exists this funtion erases the file.
 *
 * \param arena Pointer to arena to allocate the file handle.
 * \param path Path to the file
 * \param mode Access mode
 * \returns Handle to the file or error
 */
FileResult os_file_create(m_Arena *arena, String8 path, FileMode mode);

/*!
 * \brief Closes the file.
 * 
 * This funtion closes the specified file.
 * After a file is not used anymore this function should be called.
 * 
 * \param file File to close
 * \returns Potential error
 */
Result os_file_close(File *file);

/*!
 * \brief Changes the cursor position in the file.
 *
 * This function moves the file cursor at the specified position.
 * 
 * \param file File to move the cursor in
 * \pos pos The new position of the cursor
 * \returns Potential error
 */
Result os_file_jmp(File *file, u64 pos);

/*!
 * \brief Checks the file size.
 *
 * \file File to check
 * \returns File size or error
 */
U64Result os_file_size(File *file);

/*!
 * \brief Writes string to a file.
 *
 * This function writes string from the files current cursor position.
 * To change the cursor position see the os_file_jmp() function.
 * 
 * \param file File to write into
 * \param string String to write
 * \returns Potential error
 *
 * \see os_file_jmp()
 */
Result os_file_write(File *file, String8 string);

/*!
 * \brief Reads a number of bytes from a file.
 *
 * This function reads the count bytes from the files current cursor position.
 * To change the cursor position see the os_file_jmp() function.
 * 
 * \param arena
 * \param file File to read from
 * \param count Number of bytes to read
 * \returns Allcates string with the read bytes or error
 *
 * \see os_file_jmp()
 */
String8AllocResult os_file_read_count(m_Arena *arena, File *file, u64 count);

/*!
 * \brief Reads the while file from top to bottom.
 *
 * This function sets the file cursor to 0 and reads until the end.
 * The files cursor is at the end of the file after this function.
 *
 * \param arena To allocate the string
 * \param file File to read
 * \returns Read string or error
 */
String8AllocResult os_file_read_all(m_Arena *arena, File *file);

/****************************************
 * Console
****************************************/

/*!
 * \brief Writes the string to console
 *
 * \param string String to write
 * \returns Potential error
 */
Result os_console_write(String8 string);

/*!
 * \brief Reads from console until the specified charcter is found.
 *
 * \param arena Arena for allcoations
 * \param breakchar Character that stops the reading
 * \returns Allocated string that was read or error
 */
String8AllocResult os_console_read_until(m_Arena *arena, u8 breakchar);
#endif
