#include "base/mem.h"
#include "base/strings.h"
#include "base/types.h"
#include "os/os_io.h"
#include "base/errors.h"

#include <errno.h>
#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define FILE(desc) (File){.descriptor = desc}

#define DEFAULT_PERMISSIONS 0000644 /* rw-r--r-- */

struct File {
    int descriptor;
};

static mode_t
os_file_convert_mode(FileMode mode) {
    switch(mode) {
        case FILE_R:
            return O_RDONLY;
        case FILE_W:
            return O_WRONLY;
        case FILE_RW:
            return O_RDWR;
        case FILE_A: return O_WRONLY | O_APPEND;
        case FILE_AR:
            return O_RDWR | O_APPEND;
        default:
            return 0;
    }
}

bool
os_file_exists(m_Arena *arena, String8 path) {
    m_Temp temp;
    bool value = false;
    int acc;
    m_temp_begin(arena, &temp);
    {
        const char* cstr = str8_to_cstr(temp.arena, path); 
        
        acc = access(cstr, F_OK);
        if(acc == 0) {
            value = true;
        }
    }
    m_temp_end(&temp);
    return value;
}

static Error
os_file_errors(int err) {
    switch(err) {
        case EACCES:
            return PermissionError;
        case EINVAL:
            return InvalidArgumentError;
        case ENAMETOOLONG:
            return InvalidArgumentError;
        case ENOENT:
            return FileNotFoundError;
        case ENOMEM:
            return OutOfMemoryError;
        case ENOSPC:
            return OutOfDiskSpaceError;
        case EPERM:
            return PermissionError;
        default:
            return UnspecifiedError;
    }
}

FileResult
os_file_open(m_Arena *arena, String8 path, FileMode mode) {
    m_Temp temp;
    int descriptor;
    m_temp_begin(arena, &temp);
    {
        char* cstr = str8_to_cstr(temp.arena, path);
        descriptor = open(cstr, os_file_convert_mode(mode), DEFAULT_PERMISSIONS);
    }
    m_temp_end(&temp);
    
    if (descriptor < 0) {
        return (FileResult) ResultERR(os_file_errors(errno));
    }

    File *file = m_arena_alloc(arena, sizeof(*file))f;
    file->descriptor = descriptor;
    
    return (FileResult) ResultOK(file);
}

FileResult
os_file_create(m_Arena *arena, String8 path, FileMode mode) {
    m_Temp temp;
    int descriptor;
    m_temp_begin(arena, &temp);
    {
        char* cstr = str8_to_cstr(temp.arena, path);
        descriptor = open(cstr, os_file_convert_mode(mode) | O_CREAT | O_TRUNC, DEFAULT_PERMISSIONS);

    }
    m_temp_end(&temp);
    
    if(descriptor < 0) {
        return (FileResult) ResultERR(os_file_errors(errno));
    }

    File *file = m_arena_alloc(arena, sizeof(*file))f;
    file->descriptor = descriptor;
    return (FileResult) ResultOK(file);
}

Result
os_file_close(File *file) {
    int val = close(file->descriptor);
    if(val < 0) {
        switch(errno) {
            case EBADF:
                return (Result) EmptyResultOK(); /* file is already closed */
            default:
                return (Result) EmptyResultERR(UnspecifiedError);
        }
    }
    return (Result) EmptyResultOK();
}

Result
os_file_jmp(File *file, u64 pos) {
    off_t seek_out = lseek(file->descriptor, pos, SEEK_SET);
    if(seek_out < 0) {
        switch(errno) {
            case EBADF:
                return (Result) EmptyResultERR(InvalidOperationError);
            default:
                return (Result) EmptyResultERR(UnspecifiedError);
        }
    }

    return (Result) EmptyResultOK();
}

U64Result
os_file_size(File *file) {
    struct stat file_info;
    int stat_out = fstat(file->descriptor, &file_info);
    if(stat_out < 0) {
        switch(errno) {
            case EBADF:
                return (U64Result) ResultERR(InvalidOperationError);
            default:
                return (U64Result) ResultERR(UnspecifiedError);
        }
    }
    return (U64Result) ResultOK(file_info.st_size);
}

Result
os_file_write(File* file, String8 string) {
    ssize_t wr = write(file->descriptor, string.str, string.count);
    if(wr < 0) {
        switch(errno) {
            case EBADF:
                return (Result) EmptyResultERR(InvalidOperationError);
            case EFAULT:
                return (Result) EmptyResultERR(InvalidArgumentError);
            case ENOSPC:
                return (Result) EmptyResultERR(OutOfDiskSpaceError);
            case EPERM:
                return (Result) EmptyResultERR(PermissionError);
            default:
                return (Result) EmptyResultERR(UnspecifiedError);
        }
    }

    return (Result) EmptyResultOK();
}

String8AllocResult
os_file_read_count(m_Arena *arena, File *file, u64 count) {
    String8Alloc a_str = str8_alloc(arena, count);
    ssize_t read_out = read(file->descriptor,a_str.string.str, count);
    if(read_out < 0) {
        switch(errno) {
            case EBADF:
                return (String8AllocResult) ResultERR(InvalidOperationError);
            case EFAULT:
                return (String8AllocResult) ResultERR(InvalidArgumentError);
            default:
                return (String8AllocResult) ResultERR(UnspecifiedError);
        }
    }

    return (String8AllocResult) ResultOK(a_str);
}

String8AllocResult
os_file_read_all(m_Arena *arena, File* file) {
    /* jump to the start of the file */
    Result r_jmp = os_file_jmp(file, 0);
    if(!r_jmp.ok) {
        return (String8AllocResult) ResultERR(r_jmp.err); 
    }
    
    /* get the file size */
    U64Result r_size = os_file_size(file);
    if(!r_size.ok) {
        return (String8AllocResult) ResultERR(r_size.err);
    }
    
    /* read from start to file size */
    return os_file_read_count(arena, file, r_size.value);
}

Result
os_console_write(String8 string) {
    File outfile = FILE(STDOUT);
    Result r_write = os_file_write(&outfile, string);
    if(!r_write.ok) {
        return (Result) EmptyResultERR(r_write.err); 
    }
    return (Result) EmptyResultOK();
}

String8AllocResult
os_console_read_until(m_Arena *arena, u8 splitter) {
    u64 arena_pos = arena->pos;
    String8Alloc a_str = {.string = {.count = 0, .str = arena->memory + arena_pos}};
    
    bool has_nl = false;
    while(!has_nl) {
        char buf[10];

        ssize_t rd = read(STDIN, buf, 10);
        if(rd < 0) {
            return (String8AllocResult) ResultERR(UnspecifiedError);
        }
    
        u8 i;
        for(i = 0; i < rd; i++) {
            if(buf[i] == splitter) {
                has_nl = true;
                break;
            }
        }
        
        void* ptr = m_arena_alloc(arena, i)f;
        if(ptr == null) {
            m_arena_dealloc_to(arena, arena_pos);
            return (String8AllocResult) ResultERR(OutOfMemoryError);
        }

        m_copy(a_str.string.str + a_str.string.count, buf, i);
        a_str.string.count += i;
    }

    return (String8AllocResult) ResultOK(a_str);
}
