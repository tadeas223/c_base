#include "mem.h"
#include "types.h"
#include <stdio.h>

static m_MemoryBase *base_default;
void
m_memory_change_noop(void* ctx, void* ptr, u64 size) {}

void
m_memory_base_set_default(m_MemoryBase *base) {
    base_default = base;
}

m_MemoryBase*
m_memory_base_get_default() {
    return base_default;
}

u64
m_align_forward(u64 ptr, u64 align) {
    u64 modulo = ptr % align;

    if(modulo != 0) {
        ptr += align - modulo;
    }
    return ptr;
}

/* debug base wrapper */
void*
m_memory_base_debug_reserve(void *ctx, u64 size) {
    printf("RESERVE size=%llu\n", size);
    return ((m_MemoryBase*)ctx)->reserve(((m_MemoryBase*)ctx)->ctx, size);
}

void
m_memory_base_debug_commit(void *ctx, void* ptr, u64 size) {
    printf("COMMIT ptr=%p size=%llu\n", ptr, size);
    return ((m_MemoryBase*)ctx)->commit(((m_MemoryBase*)ctx)->ctx, ptr, size);
}

void
m_memory_base_debug_decommit(void *ctx, void* ptr, u64 size) {
    printf("DECOMMIT ptr=%p size=%llu\n", ptr, size);
    return ((m_MemoryBase*)ctx)->decommit(((m_MemoryBase*)ctx)->ctx, ptr, size);
}

void
m_memory_base_debug_release(void *ctx, void* ptr, u64 size) {
    printf("RELEASE ptr=%p size=%llu\n", ptr, size);
    return ((m_MemoryBase*)ctx)->release(((m_MemoryBase*)ctx)->ctx, ptr, size);
}

m_MemoryBase*
m_memory_base_debug(m_MemoryBase* base) {
    static m_MemoryBase memory = {};
    if(memory.release == null) {
        memory.reserve = m_memory_base_debug_reserve; 
        memory.commit = m_memory_base_debug_commit; 
        memory.decommit = m_memory_base_debug_decommit; 
        memory.release = m_memory_base_debug_release; 
        memory.ctx = base;
    }
    return &memory;
}
/* Arena allocator */

void
m_arena_init(m_Arena *arena) {
    m_arena_init_base(arena, base_default);
}

void
m_arena_init_reserve(m_Arena *arena, u64 reserve) {
    m_arena_init_reserve_base(arena, base_default, reserve);
}

void
m_arena_init_base(m_Arena *arena, m_MemoryBase *base) {
    arena->base = base;
    arena->memory = base->reserve(base->ctx, M_ARENA_DEFAULT_RESERVE);
    arena->cap = M_ARENA_DEFAULT_RESERVE;

    arena->pos = 0;
    arena->commit_pos = 0;
}

void
m_arena_init_reserve_base(m_Arena *arena, m_MemoryBase *base, u64 reserve) {
    arena->base = base;
    arena->memory = base->reserve(base->ctx, reserve);
    arena->cap = reserve;

    arena->pos = 0;
    arena->commit_pos = 0;
}

void*
m_arena_push(m_Arena *arena, u64 size) {
    void* result = 0;
    
    size = m_align_forward(size, M_DEFAULT_ALIGN);

    if(arena->pos + size > arena->cap) return null;
    
    if(arena->pos + size > arena->commit_pos) {
        u64 commit_size = size;
        commit_size += M_ARENA_COMMIT_BLOCK - 1;
        commit_size -= commit_size % M_ARENA_COMMIT_BLOCK;

        arena->base->commit(arena->base->ctx, arena->memory + arena->commit_pos, commit_size);
        arena->commit_pos += commit_size;
    }
    
    result = arena->memory + arena->pos + size;
    arena->pos += size;
    return result;
}

void
m_arena_pop(m_Arena *arena, u64 size) {
    u64 free_space;

    size = m_align_forward(size, M_DEFAULT_ALIGN);
    if(arena->pos - size <= 0) {
        arena->pos = 0;
    } else {
        arena->pos -= size;
    }
    
    free_space = arena->commit_pos - arena->pos;
    if(free_space >= M_ARENA_COMMIT_BLOCK) {
        u64 modulo = (free_space % M_ARENA_COMMIT_BLOCK);
        u64 decommit_size = free_space - modulo; 

        arena->base->decommit(arena->base->ctx, arena->memory + arena->pos + modulo, decommit_size);
        arena->commit_pos -= decommit_size;
    }
}

void
m_arena_cleanup(m_Arena *arena) {
    arena->base->release(arena->base->ctx, arena->memory, arena->commit_pos);
}

/* Pool allocator */
void
m_pool_init_reserve_base(m_Pool *pool, m_MemoryBase *base, u64 reserve, u64 data_size) {
    pool->base = base;
    pool->memory = base->reserve(base->ctx, reserve);

    pool->data_size = data_size;
    pool->cap = reserve;

    pool->pos = 0;
    pool->commit_pos = 0;
}

void
m_pool_init(m_Pool *pool, u64 data_size) {
    m_pool_init_reserve_base(pool, base_default, M_POOL_DEFAULT_RESERVE, data_size);
}

void
m_pool_init_reserve(m_Pool *pool, u64 reserve, u64 data_size) {
    m_pool_init_reserve_base(pool, base_default, reserve, data_size);
}

void
m_pool_init_base(m_Pool *pool, m_MemoryBase *base, u64 data_size) {
    m_pool_init_reserve_base(pool, base, M_POOL_DEFAULT_RESERVE, data_size);
}

void*
m_pool_alloc(m_Pool *pool) {
    if(pool->head) {
        void* result = pool->head;
        pool->head = pool->head->next;
        return result;
    } else {
        if(pool->commit_pos * pool->data_size > pool->cap) {
            return null;
        }
        u8 *commit_ptr = pool->memory + pool->commit_pos * pool->data_size;
        pool->base->commit(pool->base->ctx, commit_ptr, M_POOL_COMMIT_BLOCK * pool->data_size);
        m_pool_dealloc_count(pool, commit_ptr, M_POOL_COMMIT_BLOCK);
        
        pool->commit_pos += M_POOL_COMMIT_BLOCK;

        return(m_pool_alloc(pool));
    }

}

void
m_pool_dealloc(m_Pool *pool, void *ptr) {
    ((struct m_PoolFreeNode*)ptr)->next = pool->head;
    pool->head = ptr;
}

void
m_pool_dealloc_count(m_Pool *pool, void *ptr, u64 count) {
    u8 *iter = ptr;
    u64 i;
    for(i = 0; i < count; i++) {
        ((struct m_PoolFreeNode*)iter)->next = pool->head;
        pool->head = (struct m_PoolFreeNode*) iter;
        iter += pool->data_size;
    }
}

void
m_pool_cleanup(m_Pool *pool) {
    pool->base->release(pool->base->ctx, pool->memory, pool->commit_pos * pool->data_size);
}
