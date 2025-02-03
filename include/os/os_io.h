#ifndef OS_IO_H
#define OS_IO_H

#include "base/mem.h"
#include "base/strings.h"
#include "base/errors.h"

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

String8Result os_file_read_all(m_Arena *arena, File *file);
U8Result os_file_read_byte(File *file);

Result os_file_write(File *file, String8 string);

Result os_file_close(File *file);

Result os_console_writeb(u8 b);
Result os_console_writeln(String8 string);
Result os_console_write(String8 string);
u8 os_console_readb();
String8 os_console_readln(m_Arena *arena);
String8 os_console_read_until(m_Arena *arena, u8 splitter);
#endif
