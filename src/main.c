#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "os/os_mem.h"
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;

    m_arena_begin(&arena);
    {
        ConsoleWrite(Str8Lit("your name: "));
        String8Alloc a_name = ConsoleReadUntil(&arena, ';');
        
        String8List list;
        str8_list_begin(&list);
        str8_list_push(&arena, &list, Str8Lit("Hello, "));
        str8_list_push(&arena, &list, a_name.string);
        String8Alloc a_out = str8_join(&arena, &list);

        ConsoleWriteLn(a_out.string);
    }
    m_arena_end(&arena);
    return 0;
}
