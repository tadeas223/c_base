#include "base/mem.h"
#include "base/errors.h"
#include "base/types.h"

DefineError(OutOfMemoryError);

/****************************************
 * Memory Opearations
****************************************/

u64
m_align_forward(u64 ptr, u64 align) {
    u64 modulo = ptr % align;

    if(modulo != 0) {
        ptr += align - modulo;
    }
    return ptr;
}

u64
m_align_backward(u64 ptr, u64 align) {
    u64 modulo = ptr % align;

    if(modulo != 0) {
        ptr -= modulo;
    }
    return ptr;
}

void
m_copy(void *dest, void *src, u64 size) {
    if(!size) return;
    u8 *dest8 = (u8*) dest;
    u8 *src8 = (u8*) src;

    if(dest8 < src8) {
        u64 i;
        for(i = 0; i < size; i++) {
            dest8[i] = src8[i];
        }
    } else {
        while(size--) {
            dest8[size] = src8[size];
        }
    }
}


/****************************************
 * Base Memory
****************************************/

static m_MemoryBase *base_default;

void
m_memory_base_set_default(m_MemoryBase *base) {
    base_default = base;
}

m_MemoryBase*
m_memory_base_get_default() {
    return base_default;
}


/****************************************
 * 64-bit Arena - initializers
****************************************/

void
m_arena_begin(m_Arena *arena) {
    m_arena_begin_reserve_base(arena, base_default, M_ARENA_DEFAULT_RESERVE);
}

void
m_arena_begin_reserve(m_Arena *arena, u64 reserve) {
    m_arena_begin_reserve_base(arena, base_default, reserve);
}

void
m_arena_begin_base(m_Arena *arena, m_MemoryBase *base) {
    m_arena_begin_reserve_base(arena, base, M_ARENA_DEFAULT_RESERVE);
}

void
m_arena_begin_reserve_base(m_Arena *arena, m_MemoryBase *base, u64 reserve) {
    arena->base = base;
    arena->memory = base->reserve(base->ctx, reserve);
    arena->cap = reserve;

    arena->pos = 0;
    arena->commit_pos = 0;
}


/****************************************
 * 64-bit Arena - destructors
****************************************/

void
m_arena_end(m_Arena *arena) {
    arena->base->release(arena->base->ctx, arena->memory, arena->commit_pos);
}

void
m_arena_reset(m_Arena *arena) {
    arena->base->release(arena->base->ctx, arena->memory, arena->commit_pos);
    m_arena_begin_reserve_base(arena, arena->base, arena->cap);
}

/****************************************
 * 64-bit Arena - allocations
****************************************/

VoidPtrResult
m_arena_alloc(m_Arena *arena, u64 size) {
    void* result = 0;
    
    size = m_align_forward(size, M_DEFAULT_ALIGN);

    if(arena->pos + size > arena->cap) return (VoidPtrResult) ResultERR(OutOfMemoryError);
    if(arena->pos + size > arena->commit_pos) {
        u64 commit_size = size;
        commit_size += M_ARENA_COMMIT_BLOCK - 1;
        commit_size -= commit_size % M_ARENA_COMMIT_BLOCK;

        arena->base->commit(arena->base->ctx, arena->memory + arena->commit_pos, commit_size);
        arena->commit_pos += commit_size;
    }
    
    result = arena->memory + arena->pos;
    arena->pos += size;
    return (VoidPtrResult) ResultOK(result);
}

void
m_arena_dealloc(m_Arena *arena, u64 size) {
    u64 free_space;
    
    /* align forward!!! */
    size = m_align_forward(size, M_DEFAULT_ALIGN);
    
    /* overflow check */
    if(arena->pos - size > U64_MAX + size || arena->pos - size == 0) {
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
m_arena_dealloc_to(m_Arena *arena, u64 pos) {
    if(pos >= arena->pos) return; /* theres nothing to pop */
    

    pos = m_align_backward(pos, M_DEFAULT_ALIGN);
    arena->pos = pos; 
    
    u64 free_space;
    free_space = arena->commit_pos - arena->pos;
    if(free_space >= M_ARENA_COMMIT_BLOCK) {
        u64 modulo = (free_space % M_ARENA_COMMIT_BLOCK);
        u64 decommit_size = free_space - modulo; 

        arena->base->decommit(arena->base->ctx, arena->memory + arena->pos + modulo, decommit_size);
        arena->commit_pos -= decommit_size;
    }
}


/****************************************
 * Temporary Arena
****************************************/

void
m_temp_begin(m_Arena *arena, m_Temp *temp) {
    *temp = (m_Temp) {.pos = arena->pos, .arena = arena};
}

void
m_temp_end(m_Temp *temp) {
    m_arena_dealloc_to(temp->arena, temp->pos);
    *temp= (m_Temp){0};
}


/****************************************
 * 64-bit Pool - initializers
****************************************/

void
m_pool_begin(m_Pool *pool, u64 data_size) {
    m_pool_begin_reserve_base(pool, base_default, M_POOL_DEFAULT_RESERVE, data_size);
}

void
m_pool_begin_reserve(m_Pool *pool, u64 reserve, u64 data_size) {
    m_pool_begin_reserve_base(pool, base_default, reserve, data_size);
}

void
m_pool_begin_base(m_Pool *pool, m_MemoryBase *base, u64 data_size) {
    m_pool_begin_reserve_base(pool, base, M_POOL_DEFAULT_RESERVE, data_size);
}

void
m_pool_begin_reserve_base(m_Pool *pool, m_MemoryBase *base, u64 reserve, u64 data_size) {
    pool->base = base;
    pool->memory = base->reserve(base->ctx, reserve);
    
    /* align forward :( */
    pool->data_size = m_align_forward(data_size, M_DEFAULT_ALIGN);
    pool->cap = reserve;

    pool->commit_pos = 0;
    pool->head = null;
}

/****************************************
 * 64-bit Pool - destructors
****************************************/

void
m_pool_end(m_Pool *pool) {
    pool->base->release(pool->base->ctx, pool->memory, pool->commit_pos * pool->data_size);
}

void
m_pool_reset(m_Pool *pool) {
    pool->base->release(pool->base->ctx, pool->memory, pool->commit_pos * pool->data_size);
    m_pool_begin_reserve_base(pool, pool->base, pool->cap, pool->data_size);
}

/****************************************
 * 64-bit Pool - allocations
****************************************/

VoidPtrResult
m_pool_alloc(m_Pool *pool) {
    if(pool->head != null) {
        void* result = pool->head;
        pool->head = pool->head->next;
        return (VoidPtrResult) ResultOK(result);
    } else {
        if(pool->commit_pos > pool->cap) {
            return (VoidPtrResult) ResultERR(OutOfMemoryError);
        }
        u64 real_commit_block = M_POOL_COMMIT_BLOCK * pool->data_size;
        u8 *commit_ptr = pool->memory + pool->commit_pos;
        pool->base->commit(pool->base->ctx, commit_ptr, real_commit_block);
        m_pool_dealloc_count(pool, commit_ptr, M_POOL_COMMIT_BLOCK);
        
        pool->commit_pos += real_commit_block;

        return m_pool_alloc(pool);
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
