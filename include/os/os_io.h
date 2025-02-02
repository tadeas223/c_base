#ifndef OS_IO_H
#define OS_IO_H

#include "base/strings.h"
#include "base/errors.h"

typedef struct File File;

typedef struct {
    ResultVars(File*);
} FileResult;

enum FileErrorCodes {
    ERR_FILE_INVALID, /* invalid or closed file */
    ERR_IO,           /* hardware io error */
    ERR_FILE_PERMISSION_DENIED,
    ERR_FILE_NOT_FOUND,
};

enum FileMode {
    FILE_W,
    FILE_R,
    FILE_RW,
};

FileResult os_file_open(m_Arena *arena, String8 path, enum FileMode mode);
FileResult os_file_create(m_Arena *arena, String8 path, enum FileMode mode);
String8Result os_file_read_all(m_Arena *arena, File *file);
U8Result os_file_read_byte(File *file);
BoolResult os_file_close(File *file);

#endif
