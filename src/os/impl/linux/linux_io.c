#include "base/mem.h"
#include "base/strings.h"
#include "base/types.h"
#include "os/os_io.h"
#include "base/errors.h"

#include <errno.h>
#include <fcntl.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <unistd.h>


#define DEFAULT_PERMISSIONS 0000644 /* rw-r--r-- */

struct File {
    int descriptor;
    enum FileMode mode;
};

static mode_t os_file_convert_mode(enum FileMode mode) {
    switch(mode) {
        case FILE_R:
            return O_RDONLY;
        case FILE_W:
            return O_WRONLY;
        case FILE_RW:
            return O_RDWR;
        default:
            return 0;
    }
}

FileResult
os_file_open(m_Arena *arena, String8 path, enum FileMode mode) {
    m_Temp temp = m_temp_begin(arena);
    
    char* cstr = str8_to_cstr(temp.arena, path);
    int descriptor = open(cstr, os_file_convert_mode(mode), DEFAULT_PERMISSIONS);

    /* handle errors and return */
    if (descriptor == -1) {
        m_temp_end(&temp);
        switch(errno) {
            case EACCES:
                return (FileResult) ResultERR(ERR_FILE_PERMISSION_DENIED); 
            case ENOENT:
                return (FileResult) ResultERR(ERR_FILE_NOT_FOUND); 
            case EIO:
                return (FileResult) ResultERR(ERR_IO); 
            case EBADF:
                return (FileResult) ResultERR(ERR_FILE_INVALID);
            default:
                return (FileResult) ResultERR(ERR_UNSPECIFIED);
        }
    }
    
    m_temp_end(&temp);
    
    File *file = m_arena_push(arena, sizeof(*file));
    file->descriptor = descriptor; 
    return (FileResult) ResultOK(file);
}

FileResult
os_file_create(m_Arena *arena, String8 path, enum FileMode mode) {
    m_Temp temp = m_temp_begin(arena);
    
    char* cstr = str8_to_cstr(temp.arena, path);
    int descriptor = open(cstr, os_file_convert_mode(mode) | O_CREAT | O_TRUNC, DEFAULT_PERMISSIONS);

    if(descriptor == -1) {
        m_temp_end(&temp);
         
    }

    m_temp_end(&temp);
    
    File *file = m_arena_push(arena, sizeof(*file));
    file->descriptor = descriptor;
    return (FileResult) ResultOK(file);
}

U8Result
os_file_read_byte(File* file) {
    u8 byte;
    ssize_t r = read(file->descriptor, &byte, 1);
    if(r == -1) {
        switch(errno) {
            case EIO:
                return (U8Result) ResultERR(ERR_IO);
            case EBADF:
                return (U8Result) ResultERR(ERR_FILE_INVALID);
            case EINVAL:
                return (U8Result) ResultERR(ERR_FILE_INVALID);
            default:
                return (U8Result) ResultERR(ERR_UNSPECIFIED);
        }
    }
    return (U8Result) ResultOK(byte);
}

String8Result
os_file_read_all(m_Arena *arena, File* file) {
    off_t offset = lseek(file->descriptor, 0,  SEEK_SET);
    if(offset == - 1) {
        switch(errno) {
            case EBADF:
                return (String8Result) ResultERR(ERR_FILE_INVALID);
            case ENXIO:
                return (String8Result) ResultERR(ERR_IO);
            default:
                return (String8Result) ResultERR(ERR_UNSPECIFIED);
        }
    }
    
    struct stat file_info;
    int stat = fstat(file->descriptor, &file_info);
    if(stat == -1) {
        switch(errno) {
            case EBADF:
                return (String8Result) ResultERR(ERR_FILE_INVALID);
            case EIO:
                return (String8Result) ResultERR(ERR_IO);
            default:
                return (String8Result) ResultERR(ERR_UNSPECIFIED);
        }
    }

    String8 string = str8_alloc(arena, (u64)file_info.st_size);

    ssize_t r = read(file->descriptor, string.str, file_info.st_size);
    if(r == -1) {
        switch(errno) {
            case EIO:
                return (String8Result) ResultERR(ERR_IO);
            case EBADF:
                return (String8Result) ResultERR(ERR_FILE_INVALID);
            case EINVAL:
                return (String8Result) ResultERR(ERR_FILE_INVALID);
            default:
                return (String8Result) ResultERR(ERR_UNSPECIFIED);
        }
    }

    return (String8Result) ResultOK(string);
}

BoolResult
os_file_close(File *file) {
    if(close(file->descriptor) == -1) {
        switch(errno) {
            case EBADF:
                return (BoolResult) ResultERR(ERR_FILE_INVALID);
            case EIO:
                return (BoolResult) ResultERR(ERR_IO);
            default:
                return (BoolResult) ResultERR(ERR_UNSPECIFIED);
        }
    } else {
        return (BoolResult) ResultOK(true);
    }
}
