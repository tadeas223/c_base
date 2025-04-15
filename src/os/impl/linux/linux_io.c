#include "base/arena.h"
#include "base/errors.h"
#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "system.h"

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

Define_ErrorGroup(EG_OS_IO);

#define DEFAULT_PERMISSIONS 0000644 /* rw-r--r-- */

struct File {
    int descriptor;
};

static bool def_terminal_set = false;
static struct termios def_terminal;

File __file_stdin = {.descriptor = STDIN_FILENO};
File __file_stdout = {.descriptor = STDOUT_FILENO};
File __file_stderr = {.descriptor = STDERR_FILENO};

File *file_stdin = &__file_stdin;
File *file_stdout = &__file_stdout;
File *file_stderr = &__file_stderr;

static int os_file_mode(FileMode mode) {
    switch (mode) {
        case FILE_R:
            return O_RDONLY;
        case FILE_W:
            return O_WRONLY;
        case FILE_RW:
            return O_RDWR;
        case FILE_A:
            return O_APPEND|O_WRONLY;
        case FILE_AR:
            return O_APPEND|O_RDWR;
        default:
            return 0;
    }
}

bool os_file_exists(Arena *arena, String filepath) {
    bool exist;

    ArenaTemp temp;
    arena_temp_init(arena, &temp);
    {
        char* cstr_path = str_to_cstr(arena, filepath);

        int result = access(cstr_path, F_OK);
        if(result < 0) {
            exist = false; 
        } else {
            exist = true; 
        }
    }
    arena_temp_destruct(&temp);
    
    return exist;
}

Result(ptr /* File* */) os_file_open(Arena *arena, String filepath, FileMode mode) {
    int fildes;
    ArenaTemp temp;
    arena_temp_init(arena, &temp);
    {
        char* cstr_path = str_to_cstr(arena, filepath);

        fildes = open(cstr_path, os_file_mode(mode));
    }
    arena_temp_destruct(&temp);
    
    if(fildes < 0) {
        return ResultErr(ptr, E(EG_OS_IO, E_Unspecified, S("error while openning a file")));
    } else {
        File *file = arena_alloc(arena, sizeof(File));
        file->descriptor = fildes;
        return ResultOk(ptr, file);
    }
}

Result(ptr /* File* */) os_file_create(Arena *arena, String filepath, FileMode mode) {
    int fildes;

    ArenaTemp temp;
    arena_temp_init(arena, &temp);
    {
        char* cstr_path = str_to_cstr(arena, filepath);

        fildes = open(cstr_path, os_file_mode(mode)|O_CREAT|O_TRUNC, DEFAULT_PERMISSIONS);
    }
    arena_temp_destruct(&temp);
    
    if(fildes < 0) {
        return ResultErr(ptr, E(EG_OS_IO, E_Unspecified, S("error while openning a file")));
    } else {
        File *file = arena_alloc(arena, sizeof(File));
        file->descriptor = fildes;
        return ResultOk(ptr, file);
    }
}

Result(none) os_file_jmp(File* file, u64 pos) {
    off_t result = lseek(file->descriptor, pos, SEEK_SET);
    if(result < 0) {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("lseek falied")));
    }
    return ResultOk(none, null);
}

Result(none) os_file_jmp_offset(File* file, u64 pos) {
    off_t result = lseek(file->descriptor, pos, SEEK_CUR);
    if(result < 0) {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("lseek falied")));
    }
    return ResultOk(none, null);
}

Result(none) os_file_jmp_start(File* file) {
    off_t result = lseek(file->descriptor, 0, SEEK_SET);
    if(result < 0) {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("lseek falied")));
    }
    return ResultOk(none, null);
}

Result(none) os_file_jmp_end(File* file) {
    off_t result = lseek(file->descriptor, 0, SEEK_END);
    if(result < 0) {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("lseek falied")));
    }
    return ResultOk(none, null);
}

Result(u64) os_file_size(File* file) {
    struct stat st;
    int result = fstat(file->descriptor, &st);
    if(result < 0) {
        return ResultErr(u64, E(EG_OS_IO, E_Unspecified, S("fstat failed")));
    } else {
        return ResultOk(u64, st.st_size); 
    }
}

Result(none) os_file_write(File* file, String string) {
    ssize_t result = write(file->descriptor, string.chars, string.len);
    if(result == string.len){
        return ResultOk(none, null); 
    } else {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("write failed")));
    }
}

Result(u8) os_file_read(Arena *arena, File* file) {
    u8 c;
    ssize_t result = read(file->descriptor, &c, 1);
    if(result < 0) {
        return ResultErr(u8, E(EG_OS_IO, E_Unspecified, S("read failed")));
    } else if(result == 0) {
        return ResultErr(u8, E(EG_OS_IO, E_Unspecified, S("end of file reached")));
    } 
    return ResultOk(u8, c);
}

Result(StringAlloc) os_file_read_len(Arena *arena, File* file, u32 len) {
    u8 stack_buf[len]; 
    ssize_t result = read(file->descriptor, &stack_buf, len);
    if(result < 0) {
        return ResultErr(StringAlloc, E(EG_OS_IO, E_Unspecified, S("read failed")));
    } else if(result == 0) {
        return ResultErr(StringAlloc, E(EG_OS_IO, E_Unspecified, S("end of file reached")));
    } 
    
    StringAlloc a_str = str_allocate(arena, result);
    mem_copy(a_str.string.chars, stack_buf, result);
    return ResultOk(StringAlloc, a_str);

}

Result(none) os_file_close(File* file) {
    int result = close(file->descriptor);
    if(result < 0) {
        return ResultErr(none, E(EG_OS_IO, E_Unspecified, S("close failed")));
    }

    return ResultOk(none, null); 
}

void os_console_write(String string) {
    TryCrash(none, os_file_write(file_stdout, string));
}

void os_console_err_write(String string) {
    TryCrash(none, os_file_write(file_stderr, string));
}

u8 os_console_read(Arena *arena) {
    return TryCrash(u8, os_file_read(arena, file_stdin));
}

StringAlloc os_console_read_len(Arena *arena, u32 len) {
    return TryCrash(StringAlloc, os_file_read_len(arena, file_stdin, len));
}

void os_console_raw_mode(bool raw_mode) {
    if(!def_terminal_set) {
        tcgetattr(file_stdin->descriptor, &def_terminal); 
        if(!(def_terminal.c_lflag & ICANON)) {
            def_terminal.c_lflag &= ~ICANON;
        }
        def_terminal_set = true; 
    }

    
    if(raw_mode) {
        struct termios newt;
        newt = def_terminal;
        
        if(newt.c_lflag & ICANON) {
            newt.c_lflag &= ~ICANON;
        }
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(file_stdin->descriptor, TCSANOW, &newt);
    } else {
        tcsetattr(file_stdin->descriptor, TCSANOW, &def_terminal);
    }
}
