#ifndef OS_IO_H
#define OS_IO_H

#include "base/mem.h"
#include "base/strings.h"
#include "base/errors.h"

#define ConsoleWrite(string) os_console_write(string)
#define ConsoleWriteLn(string) os_console_writeln(string)
#define ConsoleWriteB(byte) os_console_writeb(byte)

#define ConsoleReadLn(arena) os_console_readln(arena).value
#define ConsoleReadUntil(arena, splitter) os_console_read_until(arena, splitter).value
#define ConsoleReadB() os_console_readb().value

typedef struct File File;

typedef struct {
    ResultVars(File*);
} FileResult;

typedef enum {
    FILE_W,
    FILE_R,
    FILE_RW,
    FILE_A, /* append mode */
    FILE_AR,
} FileMode;

bool os_file_exists(m_Arena *arena, String8 path);

FileResult os_file_open(m_Arena *arena, String8 path, FileMode mode);
FileResult os_file_create(m_Arena *arena, String8 path, FileMode mode);

U8Result os_file_readb(File *file);
String8AllocResult os_file_read_all(m_Arena *arena, File *file);
String8AllocResult os_file_read_until(m_Arena *arena, File *file, u8 splitter);
String8AllocResult os_file_readln(m_Arena *arena, File *file);

Result os_file_write(File *file, String8 string);

Result os_file_close(File *file);

/* Errors can be unchecked */
Result os_console_writeb(u8 b);
Result os_console_writeln(String8 string);
Result os_console_write(String8 string);

/* Does not return error for easy use */
U8Result os_console_readb();
String8AllocResult os_console_readln(m_Arena *arena);
String8AllocResult os_console_read_until(m_Arena *arena, u8 splitter);
#endif
