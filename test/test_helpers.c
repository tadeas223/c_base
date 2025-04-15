#include "test_helpers.h"
#include "base/arena.h"
#include "os/os_io.h"
#include "os/os_mem.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>

int test_in_new_process(void (*func)(void)) {
    pid_t pid = fork();
    if(pid == 0) {
        ConsoleWriteLn(S("[ NEW PROC ]"));
        func();
        ConsoleWriteLn(S("[ PROC END ]"));
        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);
    
    return status;
}

int test_arena_setup(void **state) {
    Arena *arena = (Arena*)state;
    arena_init_base(arena, os_memory_base());
    return 0;
}

int test_arena_teardown(void **state) {
    Arena *arena = (Arena*)state;
    arena_destruct(arena);
    return 0;
}
