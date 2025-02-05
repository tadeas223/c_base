#include "base/mem.h"
#include "base/strings.h"
#include "os/os_mem.h"
#include "os/os_io.h"
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;

    m_arena_begin(&arena);
    {
        ConsoleWrite(Str8Lit("file name: "));
        String8Alloc a_fname = ConsoleReadLn(&arena);
        
        FileResult r_file = os_file_create(&arena, a_fname.string, FILE_W);
        if(!r_file.ok) {
            ConsoleWrite(Str8Lit("Failed to open file"));
            return 1;
        }

        ConsoleWriteLn(Str8Lit("Write (exit with ~):"));
        String8Alloc a_input = ConsoleReadUntil(&arena, '~');

        if(!os_file_write(r_file.value, a_input.string).ok) {
            ConsoleWrite(Str8Lit("Failed to open file"));
            return 1;
        }

        ConsoleWriteLn(Str8Lit("File written"));

        if(!os_file_close(r_file.value).ok) {
            ConsoleWriteLn(Str8Lit("Failed to close file"));
            return 1;
        }

    }
    m_arena_end(&arena);

    return 0;
}
