#include "os/os_io.h"
#include "base/arena.h"
#include "ds/list.h"
#include "base/enviroment.h"
#include "base/mem.h"
#include "base/strdef.h"
#include "base/strings.h"
#include "base/errors.h"
#include "base/varargs.h"
#include "system.h"
#include <stdio.h>
#include <sys/stat.h>


#ifdef PLATFORM_LINUX
#include "impl/linux/linux_io.c"
#else
#error Unknown platform - os_io.c
#endif

Result(String) os_file_read_all(Arena *arena, File *file) {
    u32 size = TryReturn(String, u64, os_file_size(file)); // Casting to u32 because String length is u32
    return os_file_read_len(arena, file, size);
}

Result(String) os_file_read_ln(Arena *arena, File *file) {
#define READ_LEN 5

    List(String) list;       // List will contain StringAlloc
    ListInit(String, &list); // But to use the str_join function it needs to be String
    
    bool end = false;
    while(!end) {
        String str = TryReturn(String, String, os_file_read_len(arena, file, READ_LEN));
        if(str_find_char(str, '\n').ok) {
            end = true;
        }
    }

    return ResultOk(String, str_join(arena, list));

#undef READ_LEN
}

Result(String) os_file_one_read(Arena *arena, String filepath, u32 len) {
    File *file = TryReturn(String, ptr, os_file_open(arena, filepath, FILE_R));
    Result(String) result = os_file_read_len(arena, file, len);
    TryCrash(none, os_file_close(file));
    return result;
}

Result(String) os_file_one_read_all(Arena *arena, String filepath) {
    File *file = TryReturn(String, ptr, os_file_open(arena, filepath, FILE_R));
    Result(String) result = os_file_read_all(arena, file);
    TryCrash(none, os_file_close(file));
    return result;
}

Result(none) os_file_one_write(Arena *arena, String filepath, String string) {
    File *file = TryReturn(none, ptr, os_file_open(arena, filepath, FILE_W));
    Result(none) result = os_file_write(file, string);
    TryReturn(none, none, os_file_close(file));
    return result;
}

void os_console_write_multiple_varlist(/* String */ Varlist) {
    VarlistForeach(String, {
        os_console_write(value);
    });
}

void os_console_err_write_multiple_varlist(/* String */ Varlist) {
    VarlistForeach(String, {
        os_console_err_write(value);
    });
}

void os_console_write_multiple(/* String */ Varargs) {
    VarargsBlock({
        os_console_write_multiple_varlist(Args);
    });
}

void os_console_err_write_multiple(/* String */ Varargs) {
    VarargsBlock({
        os_console_err_write_multiple_varlist(Args);
    });
}

String os_console_read_until(Arena *arena, u8 break_char) {
#define READ_LEN 5 
    List(String) list;       // List will contain StringAlloc
    ListInit(String, &list); // But to use the str_join function it needs to be String
    
    bool end = false;
    while(!end) {
        String input = os_console_read_len(arena, READ_LEN);

        Result(u32) find = str_find_char(input, break_char);
        if(find.ok) {
            input.len = find.value; // cut the break_char and anythihng after it
            end = true;
        }
        ListPush(String, arena, &list, input);
    }

    return str_join(arena, list);
#undef READ_LEN
}

String os_console_read_ln(Arena *arena) {
    return os_console_read_until(arena, '\n');
}

void os_console_clear() {
    return os_console_write(S("\033[2J\033[H"));
}
