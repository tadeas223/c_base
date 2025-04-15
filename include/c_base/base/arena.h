
#ifndef ARENA_H
#define ARENA_H

#include "base/mem.h"
#include "base/types.h"

#define ARENA_RESERVE (u64) Gigabytes(1)
#define ARENA_COMMIT (u64) Kilobytes(8)

typedef struct {
    MemoryBase *base;

    u8* memory;
    u64 reserve;
    u64 pos;
    u64 commit_pos;
} Arena;

void arena_init_base_reserve(Arena *arena, MemoryBase* base, u64 reserve);
void arena_init_reserve(Arena *arena, u64 reserve);
void arena_init_base(Arena *arena, MemoryBase* base);
void arena_init(Arena *arena);

void* arena_alloc(Arena *arena, u64 size);

void arena_dealloc(Arena *arena, u64 size);
void arena_dealloc_to(Arena *arena, u64 pos);

void arena_reset(Arena *arena);
void arena_destruct(Arena *arena);

typedef struct {
    Arena *arena;
    u32 pos;
} ArenaTemp;

void arena_temp_init(Arena *arena, ArenaTemp *arenaTemp);
void arena_temp_destruct(ArenaTemp *arenaTemp);

#endif
