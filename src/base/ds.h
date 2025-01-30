#ifndef DS_H
#define DS_H

#include "base/mem.h"
#include "base/types.h"
#include "base/defines.h"

#define DArrayInit(array)                                \
    Stmt(                                                \
        m_Arena arena;                                   \
        m_arena_init(&arena);                            \
        m_arena_push(&arena, sizeof(*((array)->items))); \
        (array)->arena = arena;                          \
        (array)->count = 0;                              \
        (array)->items = null;                           \
    )

#define DArrayPush(array, item)                                             \
    Stmt(                                                                   \
        void* ptr = m_arena_push(&(array)->arena, sizeof(*(array)->items)); \
        if((array)->count == 0) { (array)->items = ptr; }                   \
        (array)->items[(array)->count] = item;                              \
        (array)->count++;                                                   \
    )

#define DArrayPop(array)                                       \
    Stmt (                                                     \
        m_arena_pop(&(array)->arena, sizeof(*(array)->items)); \
        (array)->count--;                                      \
        if((array)->count == 0) { (array)->items = null; }     \
    )

#define DArrayGet(array, index) (array)->items[index]

#define DArrayAdd(array, item, index)                           \
    Stmt(                                                       \
        m_arena_push(&(array)->arena, sizeof(*(array)->items)); \
        u64 i;                                                  \
        for(i = (array)->count; i > index; i--) {               \
            (array)->items[i] = (array)->items[i-1];            \
        }                                                       \
        (array)->items[index] = item;                           \
        (array)->count++;                                       \
    )

#define DArrayRemove(array, index)\
    Stmt(\
        u64 i;\
        for(i = index; i < (array)->count; i++) {\
            (array)->items[i] = (array)->items[i+1];\
        }\
        m_arena_pop(&(array)->arena, sizeof(*(array)->items));\
        (array)->count--;\
    )

#define DArrayCleanup(array) Stmt( m_arena_cleanup(&(array)->arena); )
#endif
