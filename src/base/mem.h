#ifndef MEM_H
#define MEM_H

#include "types.h"

#define M_DEFAULT_ALIGN sizeof(void*)
u64 m_align_forward(u64 ptr, u64 align);

/* base memory */
typedef void* m_ReserveFunc(void* ctx, u64 size);
typedef void m_MemoryChangeFunc(void* ctx, void* ptr, u64 size);

typedef struct {
    m_ReserveFunc *reserve;
    m_MemoryChangeFunc *commit;
    m_MemoryChangeFunc *decommit;
    m_MemoryChangeFunc *release;
    void* ctx;
} m_BaseMemory;

/* base memory helper functions */
void m_memory_change_noop(void* ctx, void* ptr, u64 size);

/* base memory malloc */
m_BaseMemory* m_base_memory_malloc();

/* 64-bit arena */
#define M_ARENA_DEFAULT_RESERVE Kilobytes(4)
#define M_ARENA_COMMIT_BLOCK 512

typedef struct {
    m_BaseMemory *base;
    u8 *memory;
    u64 pos;
    u64 commit_pos;
    u64 cap;
} m_Arena;

void m_arena_init(m_Arena *arena, m_BaseMemory *base);
void m_arena_init_reserve(m_Arena *arena, m_BaseMemory *base, u64 reserve);

void* m_arena_push(m_Arena *arena, u64 size);
void m_arena_pop(m_Arena *arena, u64 size);
#endif
