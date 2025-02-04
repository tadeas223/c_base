#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "os/os_mem.h"
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena_g;
    m_arena_begin(&arena_g);
    
    m_Arena arena;
    
    String8Alloc a_join;
    m_arena_begin(&arena);
    {
        ConsoleWrite(Str8Lit("your name: "));
        String8Alloc a_name = ConsoleReadLn(&arena);
        
        String8List list;
        str8_list_begin(&list);
        str8_list_push(&arena, &list, Str8Lit("Hello, "));
        str8_list_push(&arena, &list, a_name.string);
        a_join = str8_join(&arena, &list);

        ConsoleWriteLn(a_join.string);
    }
    m_arena_end(&arena);
    return 0;
}
