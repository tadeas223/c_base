#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "os/os_mem.h"
#include <stdlib.h>
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;

    m_arena_begin(&arena);
    {
        ConsoleWriteLn(Str8Lit("file name:"));
        String8Alloc a_fname = ConsoleReadLn(&arena);
        
        FileResult r_file = os_file_create(&arena, a_fname.string, FILE_W);
        if(!r_file.ok) {
            ConsoleWriteLn(Str8Lit("Failed to open file"));
            exit(1);
        }

        ConsoleWriteLn(Str8Lit("Write to file (exit with ~):"));
        String8Alloc a_input = ConsoleReadUntil(&arena, '~');
        Result r_write = os_file_write(r_file.value, a_input.string);
        if(!r_write.ok) {
            ConsoleWriteLn(Str8Lit("Failed to write file"));
            exit(1);
        }

        if(!os_file_close(r_file.value).ok) {
            ConsoleWriteLn(Str8Lit("Failed to close file"));
            exit(1);
        }
    }
    m_arena_end(&arena);
    return 0;
}
