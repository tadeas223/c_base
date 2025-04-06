#ifndef OS_IO_H
#define OS_IO_H

#include "base/errors.h"
#include "base/strings.h"
#include "base/types.h"

#define ConsoleWrite(args...) os_console_write_multiple(VA(args))
#define ConsoleWriteLn(args...) os_console_write_multiple(VA(args, S("\n")))

#define ConsoleRead(arena) os_console_read((arena))
#define ConsoleReadLn(arena) os_console_read_ln((arena)) 
#define ConsoleReadUntil(arena, break_char) os_console_read_until((arena), (break_char))
#define ConsoleClear() os_console_clear()
#define ConsoleFlush() os_console_flush()

TypeStructNoDecl(File, struct File);

extern File* file_stdin;
extern File* file_stdout;
extern File* file_stderr;

GenericType_Result(File_p);

Extern_ErrorGroup(EG_OS_IO);

typedef enum {
    FILE_R,
    FILE_W,
    FILE_RW,
    FILE_A,
    FILE_AR,
}FileMode ;

/****************************************
 * OS dependent stuff
****************************************/
bool os_file_exists(Arena *arena, String filepath);

Result(File_p) os_file_open(Arena *arena, String filepath, FileMode mode);
Result(File_p) os_file_create(Arena *arena, String filepath, FileMode mode);

Result(none) os_file_jmp(File* file, u64 pos);
Result(none) os_file_jmp_offset(File* file, u64 pos);
Result(none) os_file_jmp_start(File* file);
Result(none) os_file_jmp_end(File* file);

Result(u64) os_file_size(File* file);

Result(none) os_file_write_char(File* file, u8 c);
Result(none) os_file_write(File* file, String string);

Result(u8) os_file_read(Arena *arena, File* file);
Result(StringAlloc) os_file_read_len(Arena *arena, File* file, u32 len);

Result(none) os_file_close(File* file);

void os_console_write_char(u8 c);
void os_console_err_write_char(u8 c);
void os_console_write(String string);
void os_console_err_write(String string);

StringAlloc os_console_read_len(Arena *arena, u32 len);
u8 os_console_read(Arena *arena);

/****************************************
 * Higher abstractions
****************************************/

Result(StringAlloc) os_file_read_ln(Arena *arena, File* file);
Result(StringAlloc) os_file_read_until(Arena *arena, File* file);
Result(StringAlloc) os_file_read_all(Arena *arena, File* file);

Result(none) os_file_one_write(Arena *arena, String filepath, String string);
Result(StringAlloc) os_file_one_read_len(Arena *arena, String filepath, u32 len);
Result(StringAlloc) os_file_one_read_all(Arena *arena, String filepath);
Result(StringAlloc) os_file_one_read_until(Arena *arena, String filepath);


void os_console_err_write_multiple(/* String */ Varargs);
void os_console_write_multiple(/* String */ Varargs);
void os_console_err_write_multiple_varlist(/* String */ Varlist);
void os_console_write_multiple_varlist(/* String */ Varlist);

StringAlloc os_console_read_until(Arena *arena, u8 break_char);
StringAlloc os_console_read_ln(Arena *arena);

void os_console_clear();

void os_console_raw_mode(bool raw_mode);


#endif
