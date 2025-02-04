/*!**********************************************************
 * \file mem.h
 * \brief Basic memory structs and helpers
 *
 * This header includes the esentials for memory management.
************************************************************/
#ifndef MEM_H
#define MEM_H

#include "base/types.h"

#define M_DEFAULT_ALIGN sizeof(void*)


/****************************************
 * Memory Operations
****************************************/

/*!
 * \brief Aligns ptr forward.
 *
 * \param ptr value to be aligned
 * \param align the desired alignment
 */
u64 m_align_forward(u64 ptr, u64 align);
/*!
 * \brief Aligns ptr backward. 
 *
 * \param ptr value to be aligned
 * \param align the desired alignment
 */
u64 m_align_backward(u64 ptr, u64 align);
/*!
 * \brief Coppies memory from src to dest
 *
 * This function coppies the size bytes from source
 * to destination.
 *
 * When src > dest the memory is coppied from left to right 
 * 
 * When src < dest the memory is coppied from right to left 
 * 
 * \param src pointer to the memory to be coppied
 * \param dest pointer to where the memory will be coppied
 */
void m_copy(void *dest, void *src, u64 size);

/****************************************
 * Memory Base
****************************************/

/*!
 * \brief Function declaration used for reserving memory
 * 
 * \param ctx Allcoators context (Allocator will store its variables here)
 * \param size Size of the reserved memory
 *
 * \see m_MemoryBase
 */
typedef void* m_ReserveFunc(void* ctx, u64 size);
/*!
 * \brief Function declaration used for changing the state of already reserved memory
 * 
 * \param ctx Allcoators context (Allocator will store its variables here)
 * \param size Size of the memory to be commited, decomitted etc...
 * /param ptr Pointer to the memory that should change it's state
 *
 * \see m_MemoryBase
 */
typedef void m_MemoryChangeFunc(void* ctx, void* ptr, u64 size);

/*!
 * \brief Base struct for a Allocator
 * 
 * This struct holds pointers to functions that change the memory states.
 * It also holds a context to which the allocator can store its variables.
 *
 * \param reserve Pointer to the reserve function
 */
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

void m_arena_begin(m_Arena *arena);
void m_arena_begin_reserve(m_Arena *arena, u64 reserve);
void m_arena_begin_base(m_Arena *arena, m_MemoryBase *base);
void m_arena_begin_reserve_base(m_Arena *arena, m_MemoryBase *base, u64 reserve);

void m_arena_end(m_Arena *arena);
void m_arena_reset(m_Arena *arena);

void* m_arena_alloc(m_Arena *arena, u64 size);
void m_arena_dealloc(m_Arena *arena, u64 size);
void m_arena_dealloc_to(m_Arena *arena, u64 pos);

/* scatch arenas */
typedef struct {
    m_Arena *arena;
    u64 pos;
} m_Temp;

void m_temp_begin(m_Arena *arena, m_Temp *temp);
void m_temp_end(m_Temp *temp);

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

void m_pool_begin(m_Pool *pool, u64 data_size);
void m_pool_begin_reserve(m_Pool * pool, u64 reserve, u64 data_size);
void m_pool_begin_base(m_Pool *pool, m_MemoryBase *base, u64 data_size);
void m_pool_begin_reserve_base(m_Pool * pool, m_MemoryBase *base, u64 reserve, u64 data_size);

void* m_pool_alloc(m_Pool *pool);
void m_pool_dealloc(m_Pool *pool, void* ptr);
void m_pool_dealloc_count(m_Pool *pool, void* ptr, u64 count);

void m_pool_release(m_Pool *pool);
void m_pool_reset(m_Pool *pool);
#endif
