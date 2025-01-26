#include "base/base.h"
#include "base/mem.h"
#include "base/types.h"
#include "os/os.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    m_memory_base_set_default(m_memory_base_debug(os_memory_base()));
    
    m_Arena arena;
    m_arena_init_reserve(&arena, Gigabytes(1));
    u8* num_ptr0 = m_arena_push(&arena, sizeof(*num_ptr0));
    u8* num_ptr1 = m_arena_push(&arena, sizeof(*num_ptr1));
    u8* num_ptr2 = m_arena_push(&arena, sizeof(*num_ptr2));

    *num_ptr0 = 0;
    *num_ptr1 = 1;
    *num_ptr2 = 2;

    String s;
    string_init_length(&s, (u8*) "hello world", 11);
    
    String substr = substring(&s, 6, 5);

    printf("num0=%d\n", *num_ptr0);
    printf("num1=%d\n", *num_ptr1);
    printf("num2=%d\n", *num_ptr2);

    printf("string=%s\n", s.c_str);
    printf("substring=%s\n", substr.c_str);


    m_arena_pop(&arena, sizeof(*num_ptr2));

    m_arena_cleanup(&arena);

    m_Pool pool;
    m_pool_init(&pool, sizeof(u64));
    int i; 
    for(i = 0; i < 65; i++) {
        u64 *p = m_pool_alloc(&pool);
        *p = i;
        printf("p=%llu\n", *p);
        m_pool_dealloc(&pool, p); 
    }
    

    return 0;
}
