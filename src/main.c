#include "base/lists.h"
#include "base/mem.h"
#include "os/os_mem.h"
#include <stdio.h>
typedef struct IntNode {
    DNodeVars(struct IntNode);
    int value;
} IntNode;

typedef struct {
    DListVars(IntNode);
} IntList;

int main(int argc, char *argv[])
{
    m_memory_base_set_default(os_memory_base());
    m_Arena arena;
    m_arena_begin(&arena);
    {
        IntList list = {0};

        u64 i;
        for(i = 0; i < 5; i++) {
            IntNode *n = m_arena_alloc(&arena, sizeof(*n));
            n->value = i;
            DListPush(&list, n);
        }

        for(i = 5; i < 10; i++) {
            IntNode *n = m_arena_alloc(&arena, sizeof(*n));
            n->value = i;
            DListPushHead(&list, n);
        }
        IntNode *n = m_arena_alloc(&arena, sizeof(*n));
        n->value = 10;
        DListAdd(&list, 3, n);
        DListRemove(&list, 4, n);
       
        DListIter(&list, n, 
            printf("node = %d\n", n->value);
        );
    }
    m_arena_end(&arena);
    return 0;
}
