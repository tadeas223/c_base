#include "base/mem.h"
#include "base/strings.h"
#include "base/types.h"
#include "os/os_io.h"
#include "base/errors.h"

#include <fcntl.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define DEFAULT_PERMISSIONS 0000644 /* rw-r--r-- */

struct File {
    int descriptor;
    FileMode mode;
    struct stat info;
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
    m_Temp temp = m_temp_begin(arena);
    const char* cstr = str8_to_cstr(temp.arena, path); 
    
    int acc = access(cstr, F_OK);
    if(acc == 0) {
        return true; 
    }
    m_temp_end(&temp);
    return value;
}

FileResult
os_file_open(m_Arena *arena, String8 path, FileMode mode) {
    m_Temp temp = m_temp_begin(arena);
    
    char* cstr = str8_to_cstr(temp.arena, path);
    int descriptor = open(cstr, os_file_convert_mode(mode), DEFAULT_PERMISSIONS);

    if (descriptor < 0) {
        m_temp_end(&temp);
        return (FileResult) ResultERR(ERR_UNSPECIFIED); 
    }
    
    m_temp_end(&temp);
    
    File *file = m_arena_push(arena, sizeof(*file));
    file->descriptor = descriptor;
    file->mode = mode;
    
    /* check if the oppened stream is a regular file */
    bool is_reg = true;
    int inf = fstat(file->descriptor, &file->info);
    if(inf < 0) {
        is_reg = false; 
    }
    if(!S_ISREG(file->info.st_mode)) {
        is_reg = false; 
    }
    
    if(is_reg) {
        return (FileResult) ResultERR(ERR_UNSPECIFIED); 
    } else {
        return (FileResult) ResultOK(file);
    }
}

FileResult
os_file_create(m_Arena *arena, String8 path, FileMode mode) {
    m_Temp temp = m_temp_begin(arena);
    
    char* cstr = str8_to_cstr(temp.arena, path);
    int descriptor = open(cstr, os_file_convert_mode(mode) | O_CREAT | O_TRUNC, DEFAULT_PERMISSIONS);

    if(descriptor == -1) {
        m_temp_end(&temp);
        return (FileResult) ResultERR(ERR_UNSPECIFIED);    
    }

    m_temp_end(&temp);
    
    File *file = m_arena_push(arena, sizeof(*file));
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

String8Result
os_file_read_all(m_Arena *arena, File* file) {
    off_t offset = lseek(file->descriptor, 0,  SEEK_SET);
    if(offset == - 1) {
        return (String8Result) ResultERR(ERR_UNSPECIFIED); 
    }
    
    struct stat file_info;
    int stat = fstat(file->descriptor, &file_info);
    if(stat == -1) {
        return (String8Result) ResultERR(ERR_UNSPECIFIED);
    }

    String8 string = str8_alloc(arena, (u64)file_info.st_size);

    ssize_t r = read(file->descriptor, string.str, file_info.st_size);
    if(r == -1) {
        return (String8Result) ResultERR(ERR_UNSPECIFIED);
    }

    return (String8Result) ResultOK(string);
}

String8Result
os_file_read_until(m_Arena *arena, File* file, u8 splitter) {
    String8 string = {.count = 0, .str = arena->memory + arena->pos};

    bool has_nl = false;
    while(!has_nl) {
        char buf[10];
        ssize_t rd = read(file->descriptor, buf, 10);
        if(rd < 0) {
            return (String8Result) ResultERR(ERR_UNSPECIFIED);
        }
    
        u8 i;
        for(i = 0; i < 10; i++) {
            if(buf[i] == splitter) {
                has_nl = true;
                break;
            }
        }
        
        m_arena_push(arena, i);
        m_copy(string.str + string.count, buf, i);
        string.count += i;
    }

    return (String8Result) ResultOK(string);
}

String8Result
os_file_readln(m_Arena *arena, File* file) {
    return os_file_read_until(arena, file, '\n');
}

Result
os_file_write(File* file, String8 string) {
    ssize_t wr = write(file->descriptor, string.str, string.count);
    if(wr == -1) {
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

String8
os_console_read_until(m_Arena *arena, u8 splitter) {
    String8 string = {.count = 0, .str = arena->memory + arena->pos};

    bool has_nl = false;
    while(!has_nl) {
        char buf[10];
        ssize_t rd = read(STDIN, buf, 10);
        if(rd < 0) {
            return Str8Lit("STDOUT ERROR");
        }
    
        u8 i;
        for(i = 0; i < 10; i++) {
            if(buf[i] == splitter) {
                has_nl = true;
                break;
            }
        }
        
        m_arena_push(arena, i);
        m_copy(string.str + string.count, buf, i);
        string.count += i;
    }

    return string;
}

String8
os_console_readln(m_Arena *arena) {
    return os_console_read_until(arena, '\n');
}

u8
os_console_readb() {
    u8 buf = 0;
    read(STDIN, &buf, 1);
    return buf;
}
