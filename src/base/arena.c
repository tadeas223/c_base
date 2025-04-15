#include "base/arena.h"
#include "base/errors.h"
#include "base/mem.h"
#include "system.h"
#include <stdio.h>

void
arena_init_base_reserve(Arena *arena, MemoryBase *base, u64 reserve) {    
    arena->memory = TryCrash(ptr, base->reserve(base, reserve));
    
    arena->base = base;
    arena->reserve = reserve;

    arena->pos = 0;
    arena->commit_pos = 0;
}

void
arena_init_base(Arena *arena, MemoryBase *base) {
    arena_init_base_reserve(arena, base, ARENA_RESERVE);
}

void
arena_init_reserve(Arena *arena, u64 reserve) {
    arena_init_base_reserve(arena, global_memory_base_get(), ARENA_RESERVE);
}

void
arena_init(Arena *arena) {
    arena_init_base_reserve(arena, global_memory_base_get(), ARENA_RESERVE);
}

void
arena_destruct(Arena *arena) {
    TryCrash(none, arena->base->release(arena->base, arena->memory, arena->commit_pos));
}

void
arena_reset(Arena *arena) {
    arena_destruct(arena);
    arena_init_base_reserve(arena, arena->base, arena->reserve);
}

void*
arena_alloc(Arena *arena, u64 size) {
    size = mem_align_forward(size, MEM_ALIGN);

    if(size + arena->pos > arena->commit_pos) {
        u64 commit_size = size;
        commit_size += ARENA_COMMIT - 1;
        commit_size -= commit_size % ARENA_COMMIT;
            
        if(arena->commit_pos >= arena->reserve) {
            crash(E(EG_Memory, E_OutOfMemory, S("Arena is out of memory"))); 
        }

        TryCrash(none, arena->base->commit(arena->base, arena->memory + arena->commit_pos, commit_size));
        arena->commit_pos += commit_size;
    }

    void* ptr = arena->memory + arena->pos;
    arena->pos += size;

    return ptr;
}

void
arena_dealloc(Arena *arena, u64 size) {
    size = mem_align_forward(size, MEM_ALIGN);
    
    if(arena->pos - size > u64_MAX + size) {
        arena->pos = 0; 
    } else {
        arena->pos -= size; 
    }

    u64 free_space = arena->commit_pos - arena->pos;
    if(free_space >= ARENA_COMMIT) {
        u64 modulo = free_space % ARENA_COMMIT;
        u64 decommit_size = free_space - modulo;

        TryCrash(none, arena->base->decommit(arena->base, arena->memory + arena->pos + modulo, decommit_size));
        arena->commit_pos -= decommit_size;
    }
}

void
arena_dealloc_to(Arena *arena, u64 pos) {
    pos = mem_align_backward(pos, MEM_ALIGN);

    if(pos >= arena->pos) {
        crash(E(EG_Memory, E_OutOfMemory, S("pos is outside of the arena"))); 
    } else if(pos != mem_align_forward(pos, MEM_ALIGN)) {
        crash(E(EG_Memory, E_InvalidArgument, S("pos is not properly aligned"))); 
    }

    arena->pos = pos;

    u64 free_space = arena->commit_pos - arena->pos;
    if(free_space >= ARENA_COMMIT) {
        u64 modulo = free_space % ARENA_COMMIT;
        u64 decommit_size = free_space - modulo;
        
        TryCrash(none, arena->base->decommit(arena->base, arena->memory + arena->pos + modulo, decommit_size));
        arena->commit_pos -= decommit_size;
    }
}


void arena_temp_init(Arena *arena, ArenaTemp *arenaTemp) {
    arenaTemp->arena = arena;
    arenaTemp->pos = arena->pos;
}

void arena_temp_destruct(ArenaTemp *arenaTemp) {
    arena_dealloc_to(arenaTemp->arena, arenaTemp->pos);
}
