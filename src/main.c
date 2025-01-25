#include "base/defines.h"
#include "base/mem.h"
#include "base/types.h"
#include "os/os_mem.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    m_Arena arena;
    m_arena_init(&arena, os_memory_base());
    void* ptr = m_arena_push(&arena, 64);
    void* ptr2 = m_arena_push(&arena, Kilobytes(2));
    printf("%p\n", ptr);
    printf("%p\n", ptr2);
    
    m_arena_pop(&arena, Kilobytes(2));
    m_arena_pop(&arena, 64);
    m_arena_(&arena);
    return 0;
}
