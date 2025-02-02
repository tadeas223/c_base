#include "base/mem.h"
#include "base/strings.h"
#include "os/os_mem.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;
    m_arena_init(&arena);

    String8 str = Str8Lit("hello");

    printf("%s\n", str8_to_cstr(&arena, str));
    return 0;
}
