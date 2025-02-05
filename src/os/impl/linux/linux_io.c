#include "base/mem.h"
#include "base/strings.h"
#include "base/types.h"
#include "os/os_io.h"
#include "base/errors.h"

#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define DEFAULT_PERMISSIONS 0000644 /* rw-r--r-- */

struct File {
    int descriptor;
};

static mode_t os_file_convert_mode(FileMode mode) {
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

bool os_file_exists(m_Arena *arena, String8 path) {
    bool value = false;
    m_Temp temp;
    
    m_temp_begin(arena, &temp);
    {
        const char* cstr = str8_to_cstr(temp.arena, path); 
        
        int acc = access(cstr, F_OK);
        if(acc == 0) {
            value = true; 
        }
    }
    m_temp_end(&temp);

    return value;
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
        return (FileResult) ResultERR(ERR_UNSPECIFIED); 
    }

    File *file = m_arena_alloc(arena, sizeof(*file));
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
    
    if(descriptor == -1) {
        return (FileResult) ResultERR(ERR_UNSPECIFIED);    
    }

    File *file = m_arena_alloc(arena, sizeof(*file));
    file->descriptor = descriptor;
    return (FileResult) ResultOK(file);
}

U8Result
os_file_readb(File* file) {
    u8 byte;
    ssize_t r = read(file->descriptor, &byte, 1);
    if(r == -1) {
        return (U8Result) ResultERR(ERR_UNSPECIFIED); 
    }
    return (U8Result) ResultOK(byte);
}

String8AllocResult
os_file_read_all(m_Arena *arena, File* file) {
    off_t offset = lseek(file->descriptor, 0,  SEEK_SET);
    if(offset == - 1) {
        return (String8AllocResult) ResultERR(ERR_UNSPECIFIED); 
    }
    
    struct stat file_info;
    int stat = fstat(file->descriptor, &file_info);
    if(stat == -1) {
        return (String8AllocResult) ResultERR(ERR_UNSPECIFIED);
    }

    String8Alloc a_str = str8_alloc(arena, (u64)file_info.st_size);

    ssize_t r = read(file->descriptor, a_str.string.str, file_info.st_size);
    if(r == -1) {
        return (String8AllocResult) ResultERR(ERR_UNSPECIFIED);
    }

    return (String8AllocResult) ResultOK(a_str);
}

String8AllocResult
os_file_read_until(m_Arena *arena, File* file, u8 splitter) {
    String8Alloc a_str = {.string = {.count = 0, .str = arena->memory + arena->pos}};

    bool has_nl = false;
    while(!has_nl) {
        char buf[10];
        ssize_t rd = read(file->descriptor, buf, 10);
        if(rd < 0) {
            return (String8AllocResult) ResultERR(ERR_UNSPECIFIED);
        }
    
        u8 i;
        for(i = 0; i < 10; i++) {
            if(buf[i] == splitter) {
                has_nl = true;
                break;
            }
        }
        
        m_arena_alloc(arena, i);
        m_copy(a_str.string.str + a_str.string.count, buf, i);
        a_str.string.count += i;
    }

    return (String8AllocResult) ResultOK(a_str);
}

String8AllocResult
os_file_readln(m_Arena *arena, File* file) {
    return os_file_read_until(arena, file, '\n');
}

Result
os_file_write(File* file, String8 string) {
    ssize_t wr = write(file->descriptor, string.str, string.count);
    if(wr == -1 || wr < string.count) {
        return (Result) EmptyResultERR(ERR_UNSPECIFIED); 
    }

    return (Result) EmptyResultOK();
}

Result
os_file_writeln(m_Arena *arena, String8 string, File* file) {
    Result res1 = os_file_write(file, string);
    Result res2 = os_file_write(file, Str8Lit("\n"));
    
    if(!res1.ok) return res1;
    if(!res2.ok) return res2;

    return (Result) EmptyResultOK();
}

Result
os_file_close(File *file) {
    if(close(file->descriptor) == -1) {
        return (Result) EmptyResultERR(ERR_UNSPECIFIED);
    } else {
        return (Result) EmptyResultOK();
    }
}

Result
os_console_write(String8 string) {
    ssize_t wr = write(STDOUT, string.str, string.count);

    if(wr < 0) {
        return (Result) EmptyResultERR(ERR_UNSPECIFIED); 
    }
   
    return (Result) EmptyResultOK();
}

Result
os_console_writeln(String8 string) {
    ssize_t wr = write(STDOUT, string.str, string.count);
    u8 new_line = '\n';
    ssize_t wr2 = write(STDOUT, &new_line, 1);
    
    if(wr < 0 || wr2 < 0) {
        return (Result) EmptyResultERR(ERR_UNSPECIFIED);
    }
    
    return (Result) EmptyResultOK();
}

Result
os_console_writeb(u8 b) {
    if(write(STDOUT, &b, 1) < 0) {
        return (Result) EmptyResultERR(ERR_UNSPECIFIED); 
    }
   
    return (Result) EmptyResultOK();
}

String8AllocResult
os_console_read_until(m_Arena *arena, u8 splitter) {
    String8Alloc a_str = {.string = {.count = 0, .str = arena->memory + arena->pos}};

    bool has_nl = false;
    while(!has_nl) {
        char buf[10];

        ssize_t rd = read(STDIN, buf, 10);
        if(rd < 0) {
            return (String8AllocResult) ResultERR(ERR_UNSPECIFIED);
        }
    
        u8 i;
        for(i = 0; i < rd; i++) {
            if(buf[i] == splitter) {
                has_nl = true;
                break;
            }
        }
        
        m_arena_alloc(arena, i);
        m_copy(a_str.string.str + a_str.string.count, buf, i);
        a_str.string.count += i;
    }

    return (String8AllocResult) ResultOK(a_str);
}

String8AllocResult
os_console_readln(m_Arena *arena) {
    return os_console_read_until(arena, '\n');
}

U8Result
os_console_readb() {
    u8 buf = 0;
    if(read(STDIN, &buf, 1) < 0) {
        return (U8Result) ResultERR(ERR_UNSPECIFIED);
    }
    return (U8Result) ResultOK(buf);
}
