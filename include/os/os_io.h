#ifndef OS_IO_H
#define OS_IO_H

#include "base/mem.h"
#include "base/strings.h"
#include "base/errors.h"

#define ConsoleWrite(string) os_console_write(string);
#define ConsoleWriteLn(string) os_console_write(string); os_console_write(Str8Lit("\n"));

#define ConsoleReadLn(arena) os_console_read_until(arena, '\n').value
#define ConsoleReadUntil(arena, splitter) os_console_read_until(arena, splitter).value

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

Result os_file_close(File *file);

Result os_file_jmp(File *file, u64 pos);
U64Result os_file_size(File *file);

Result os_file_write(File *file, String8 string);

String8AllocResult os_file_read_count(m_Arena *arena, File *file, u64 count);
String8AllocResult os_file_read_all(m_Arena *arena, File *file);

Result os_console_write(String8 string);
String8AllocResult os_console_read_until(m_Arena *arena, u8 breakchar);
#endif
