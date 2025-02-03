#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "os/os_mem.h"
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;
    m_arena_init(&arena);
    
    os_console_writeln(Str8Lit("Hello New Line "));
    String8 string = os_console_readln(&arena);
    
    os_console_write(Str8Lit("You wrote: "));
    os_console_writeln(string);
    return 0;
}
