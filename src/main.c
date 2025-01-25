#include "base/base.h"
#include "os/os.h"

#include <stdio.h>
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    
    m_Arena arena;
    m_arena_init(&arena);

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

    return 0;
}
