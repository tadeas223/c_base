#ifndef OS_IO_H
#define OS_IO_H

#include "base/strings.h"
typedef struct File File;

enum FileMode {
    FILE_WRITE,
    FILE_READ,
};

File os_io_open(m_Arena *arena, String8 path, enum FileMode mode);
File os_io_create(m_Arena *arena, String8 path);
void os_io_close(File file);

#endif
