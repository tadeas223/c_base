#ifndef MEM_H
#define MEM_H

#include "types.h"

#define M_DEFAULT_ALIGN sizeof(void*)
u64 m_align_forward(u64 ptr, u64 align);

/* memory base */
typedef void* m_ReserveFunc(void* ctx, u64 size);
typedef void m_MemoryChangeFunc(void* ctx, void* ptr, u64 size);

typedef struct {
    m_ReserveFunc *reserve;
    m_MemoryChangeFunc *commit;
    m_MemoryChangeFunc *decommit;
    m_MemoryChangeFunc *release;
    void* ctx;
} m_MemoryBase;

/* base memory helper functions */
void m_memory_change_noop(void* ctx, void* ptr, u64 size);

/* base select */
void m_memory_base_set_default(m_MemoryBase *base);
m_MemoryBase* m_memory_base_get_default();

/* debug */
m_MemoryBase* m_memory_base_debug(m_MemoryBase *base);
void* m_memory_base_debug_reserve(void* ctx, u64 size);
void m_memory_base_debug_commit(void* ctx, void* ptr, u64 size);
void m_memory_base_debug_decommit(void* ctx, void* ptr, u64 size);
void m_memory_base_debug_release(void* ctx, void* ptr, u64 size);

/* 64-bit arena */
#define M_ARENA_DEFAULT_RESERVE Gigabytes(1)
#define M_ARENA_COMMIT_BLOCK Kilobytes(8)

typedef struct {
    m_MemoryBase *base;
    u8 *memory;
    u64 pos;
    u64 commit_pos;
    u64 cap;
} m_Arena;

void m_arena_init(m_Arena *arena);
void m_arena_init_reserve(m_Arena *arena, u64 reserve);
void m_arena_init_base(m_Arena *arena, m_MemoryBase *base);
void m_arena_init_reserve_base(m_Arena *arena, m_MemoryBase *base, u64 reserve);

void m_arena_cleanup(m_Arena *arena);

void* m_arena_push(m_Arena *arena, u64 size);
void m_arena_pop(m_Arena *arena, u64 size);

/* 64-bit Pool */
#define M_POOL_DEFAULT_RESERVE Gigabytes(1)
#define M_POOL_COMMIT_BLOCK 32 /* number of elements that can be stored in the pool */

struct m_PoolFreeNode {
    struct m_PoolFreeNode *next;
};

typedef struct {
    m_MemoryBase *base;
    u8 *memory;
    u64 cap; 
    u64 pos;
    u64 commit_pos;
    u64 data_size;
    struct m_PoolFreeNode *head;    
} m_Pool;

void m_pool_init(m_Pool *pool, u64 data_size);
void m_pool_init_reserve(m_Pool * pool, u64 reserve, u64 data_size);
void m_pool_init_base(m_Pool *pool, m_MemoryBase *base, u64 data_size);
void m_pool_init_reserve_base(m_Pool * pool, m_MemoryBase *base, u64 reserve, u64 data_size);

void* m_pool_alloc(m_Pool *pool);
void m_pool_dealloc(m_Pool *pool, void* ptr);
void m_pool_dealloc_count(m_Pool *pool, void* ptr, u64 count);
void m_pool_cleanup(m_Pool *pool);
#endif
