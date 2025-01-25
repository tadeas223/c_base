#include "mem.h"
#include "types.h"

void
m_memory_change_noop(void* ctx, void* ptr, u64 size) {}

u64
m_align_forward(u64 ptr, u64 align) {
    u64 modulo = ptr % align;

    if(modulo != 0) {
        ptr += align - modulo;
    }
    return ptr;
}

void
m_arena_init(m_Arena *arena, m_MemoryBase *base) {
    arena->base = base;
    arena->memory = base->reserve(base->ctx, M_ARENA_DEFAULT_RESERVE);
    arena->cap = M_ARENA_DEFAULT_RESERVE;

    arena->pos = 0;
    arena->commit_pos = 0;
}

void
m_arena_init_reserve(m_Arena *arena, m_MemoryBase *base, u64 reserve) {
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
