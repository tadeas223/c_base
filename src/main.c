#include "base/mem.h"
#include "base/types.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    m_Arena arena;
    m_arena_init(&arena, m_base_memory_malloc());
    void* ptr = m_arena_push(&arena, 64);
    void* ptr2 = m_arena_push(&arena, Kilobytes(2));
    printf("%p\n", ptr);
    printf("%p\n", ptr2);
    
    m_arena_pop(&arena, Kilobytes(2));
    m_arena_pop(&arena, 64);
    return 0;
}
