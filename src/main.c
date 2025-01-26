#include "base/strings.h"
#include "base/list.h"
#include "base/mem.h"
#include "base/types.h"
#include "os/os_mem.h"
#include <stdio.h>

ListPrototype(int);
ListImpl(int);

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

    m_Pool list_pool;
    List(int) list;
    ListInit(int, &list, &list_pool);
    
    for(i = 0; i < 10; i++) {
        ListPush(int, &list, i);
    }

    printf("list length=%llu\n", ListLength(&list));
    for(i = 0; i < ListLength(&list); i++) {
        printf("list get %d=%d\n", i, ListGet(int, &list, i));
    }
    
    int len = ListLength(&list);
    for(i = 0; i < len; i++) {
        printf("list pop %d=%d\n", i, ListPop(int, &list));
    }
    return 0;
}
