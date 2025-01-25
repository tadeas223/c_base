#include "base/mem.h"
#include "base/types.h"
#include "os/os_mem.h"

int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());

    m_Arena arena;
    m_arena_init(&arena);
    
    m_arena_pop(&arena, Kilobytes(2));
    m_arena_pop(&arena, 64);
    m_arena_cleanup(&arena);
    return 0;
}
