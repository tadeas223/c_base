#include "base/mem.h"
#include "base/strings.h"
#include "os/os_mem.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());

    m_Arena arena;
    m_arena_init(&arena);
    
    String8 str = str8_cstr((u8*)"hello.world");   
    
    String8View view = str8_view(&str);
    printf("view: %llu\n", view.count);   
    printf("str: %llu\n", str.count);   
    String8View substr = str8_substr(&view, 2, 5);
    
    str8_print(&str);
    printf("\n");   
    str8_print_view(&substr);
    printf("\n");   

    String8ViewList *list = str8_split(&arena, &view, '.');
   
     
    str8_print_view(&list->head->str);
    printf("\n");   
    str8_print_view(&list->tail->str);
    printf("\n");   
    return 0;
}
