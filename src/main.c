#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"
#include "os/os_mem.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;
    m_arena_init(&arena);

    FileResult file = os_file_open(&arena, Str8Lit("./test.txt"), FILE_R);
    if(!file.ok) {
        printf("Failed to open file\n");
    }

    String8Result contents = os_file_read_all(&arena, file.value);
    if(!contents.ok) {
        printf("Failed to read file\n");
    } else {
        str8_print(contents.value);
        printf("\n");
    }

    if(!os_file_close(file.value).ok) {
        printf("Failed to close file\n");
    }
    
    String8Result content = os_file_read_all(&arena, file.value);
    if(!content.ok) {
        printf("Failed to read file\n");
    } else {
        str8_print(content.value);
        printf("\n");
    }
    return 0;
}
