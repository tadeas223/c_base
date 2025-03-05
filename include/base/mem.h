/*!**********************************************************
 * \file mem.h
 * \brief Basic memory structs and helpers
 *
 * This header includes the esentials for memory management.
************************************************************/
#ifndef MEM_H
#define MEM_H

#include "base/errors.h"

/*!
 * \brief Error indication that the system or allocator is out of memory.
 */
ExternError(OutOfMemoryError);

/*!
 * \brief Default memory alignment that allocators should use.
 */
#define M_DEFAULT_ALIGN sizeof(void*)


/****************************************
 * Memory Operations
 * 
 ****************************************/

/*!
 * \brief Aligns ptr forward.
 *
 * \param ptr value to be aligned
 * \param align the desired alignment
 *
 * \returns The alligned value
 */
u64 m_align_forward(u64 ptr, u64 align);
/*!
 * \brief Aligns ptr backward. 
 *
 * \param ptr value to be aligned
 * \param align the desired alignment
 *
 * \returns The alligned value
 */
u64 m_align_backward(u64 ptr, u64 align);
/*!
 * \brief Coppies memory from src to dest
 *
 * This function coppies the size bytes from source
 * to destination.
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
 * \param ctx Allcoators context (Allocator will store it's variables here)
 * \param size Size of the reserved memory
 *
 * \see m_MemoryBase
 */
typedef void* m_ReserveFunc(void* ctx, u64 size);
/*!
 * \brief Function declaration used for changing the state of already reserved memory
 * 
 * \param ctx Allcator context (Allocator will store it's variables here)
 * \param size Size of the memory to be commited, decomitted etc...
 * \param ptr Pointer to the memory that should change it's state
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
 * \param commit Pointer to the memory commit function
 * \param decommit Pointer to the memory decommit function
 * \param release Pointer to the release function
 * \param ctx Allocator context 
 */
typedef struct {
    m_ReserveFunc *reserve;
    m_MemoryChangeFunc *commit;
    m_MemoryChangeFunc *decommit;
    m_MemoryChangeFunc *release;
    void* ctx;
} m_MemoryBase;

/*!
 * \brief Setter for the global memory base.
 *
 * This function sets the default memory base that all the other 
 * allocator should use.
 *
 * \warning This setter should be called at the start of the main() function.
 * 
 * \param base Pointer to the new memory base
 */
void m_memory_base_set_default(m_MemoryBase *base);

/*!
 * \brief Getter for the global memory base.
 *
 * This function returns the current default memory base.
 * This can be helpful when developing a memory allocator
 * based on the m_MemoryBase struct.
 * 
 * \returns The current memory base
 */
m_MemoryBase* m_memory_base_get_default();

/************************************************************
 * 64-bit Arena
************************************************************/

/*!
 * \brief The default amount of memory that the will be reserved by the arena.
 */
#define M_ARENA_DEFAULT_RESERVE Gigabytes(1)

/*!
 * \brief The default block of memory that the arena will allocate if more memory is needed.
 */
#define M_ARENA_COMMIT_BLOCK Kilobytes(8)

/*!
 * \brief Struct representing the Arena allocator.
 *
 * \param base Pointer to the memory base that actually makes the allcations
 * \param memory Memory pool to which the arena allcoates
 * \param pos Position of the end of the used space in the memory pool
 * \param commit_pos Position of the end of the commited space
 * \param cap Ammout of memory that was reserved for the arena
 */
typedef struct {
    m_MemoryBase *base;
    u8 *memory;
    u64 pos;
    u64 commit_pos;
    u64 cap;
} m_Arena;

/****************************************
 * 64-bit Arena - initializers
****************************************/

/*!
 * \brief Initializes the m_Arena with the default memory base and M_ARENA_DEFAULT_RESERVE.
 * 
 * \param arena Pointer to the arena to be initialized
 */
void m_arena_begin(m_Arena *arena);
/*!
 * \brief Initialized the m_Arena with the default memory base and a custom reserve amount.
 *
 * The reserved memory is saved in the cap variable in the m_Arena struct.
 *
 * \param arena Pointer to the arena to be initialized
 * \param reserve The amout to be reserved in bytes
 */
void m_arena_begin_reserve(m_Arena *arena, u64 reserve);

/*!
 * \brief Initialized the m_Arena with a custom memory base adn M_ARENA_DEFAULT_RESERVE.
 * 
 * \param arena Pointer to the arena to be initialized
 * \param base Custom memory base that all the allocations will go trough
 */
void m_arena_begin_base(m_Arena *arena, m_MemoryBase *base);

/*!
 * \brief Initialized the m_Arena with a custom memory base and a custom reserve amount.
 *
 * \param arena Pointer to the arena to be initialized
 * \param base Custom memory base that all the allocations will go trough
 * \param reserve The amount to be reserved in bytes
 */
void m_arena_begin_reserve_base(m_Arena *arena, m_MemoryBase *base, u64 reserve);


/****************************************
 * 64-bit Arena - destructors
****************************************/

/*!
 * \brief Releases all the memory used by the arena
 *
 * \warning To use the arena after this function reinitialize it with the m_arena_begin functions or use the m_arena_reset() function instead.
 * 
 * \see m_arena_reset()
 */
void m_arena_end(m_Arena *arena);

/*!
 * \brief Releases all the memory used by the arena and reinitializes it.
 *
 * The arena is useable even after this function.
 */
void m_arena_reset(m_Arena *arena);


/****************************************
 * 64-bit Arena - allocations
****************************************/

/*!
 * \brief Allocates memory in the m_Arena
 *
 * This functions pushes the size amount of memory to the end of m_Arena.
 * When the memory exceeds the commited memory size, the M_ARENA_COMMIT_BLOCK amount of memory
 * is commited at the end of the last commit.
 *
 * If the memory exeeds the arena reserved memory (the cap variable in m_Arena) this functions returns null.
 *
 * \param arena Arena in which the memory will be allocated
 * \param size The amount of memory to allocate
 *
 * \returns Pointer to the start of the allocated memory or null when no more memory is reserved.
 */
VoidPtrResult m_arena_alloc(m_Arena *arena, u64 size);

/*!
 * \brief Deallocates memory from the m_Arena
 *
 * This function pops the size amount of memory from the end of the arena.
 * When there is more free memory than M_ARENA_COMMIT_BLOCK that memory is decommited.
 *
 * \pre Pointer must reside in the m_Arenas memory pool.
 *
 * \param arena Pointer to the arena from which the memory will be deallocated
 * \param size The amount of memory to deallocate
 */
void m_arena_dealloc(m_Arena *arena, u64 size);

/*!
 * \brief Changes the m_Arena pos and deallocates everyting after it.
 *
 * When the pos minus commit_pos is grater than the M_ARENA_COMMIT_BLOCK
 * the commit block is decommited.
 *
 * \param arena Arena to dealocate from
 * \param pos New Arena pos
 */
void m_arena_dealloc_to(m_Arena *arena, u64 pos);

/****************************************
 * Temporary Arenas
****************************************/

/*!
 * \brief Temporary arena allocator.
 *
 * This struct holds a m_Arena and the pos inside
 * this arena.
 *
 * This can be used for temporary dynamic allocations
 * that do not need to live for very long.
 * 
 * \param arena The main arena
 * \param pos Position inside the arena (the temp arena starts from here)
 */
typedef struct {
    m_Arena *arena;
    u64 pos;
} m_Temp;

/*!
 * \brief Initializes a m_Temp arena.
 *
 * This function stores the reference to the specified m_Arena and
 * the m_Arena's pos variable into the m_Temp struct.
 *
 * \param arena Main arena
 * \param pos Position inside the arena and the starting position of the temp arena
 */
void m_temp_begin(m_Arena *arena, m_Temp *temp);

/*!
 * \brief Releases the m_Temp arena
 *
 * This function calls the m_arena_pop_to() function of the main arena
 * to the pos variable.
 * 
 * The temporary arena is useable after this function, but be sure to end it again afterwrds.

 * \param temp Temporary arena to be released
 */
void m_temp_end(m_Temp *temp);


/************************************************************
 * 64-bit Pool
************************************************************/

/*!
 * \brief The default amount of memory that the will be reserved by the pool.
 */

#define M_POOL_DEFAULT_RESERVE Gigabytes(1)
/*!
 * \brief The default number of elements that the pool allocates if more memory is needed.
 */
#define M_POOL_COMMIT_BLOCK 32

struct m_PoolFreeNode {
    struct m_PoolFreeNode *next;
};

/*!
 * \brief Struct representing the pool allocator
 *
 * \base Memory base that will do the allocations
 * \memory Memory in which the elements are stored
 * \cap Maximum memory in bytes that can be stored in the pool
 * \commit_pos Current position of the commited memory in bytes
 * \data_size Size of the memory cells
 * \head Start of the free node list
 */
typedef struct {
    m_MemoryBase *base;
    u8 *memory;
    u64 cap; 
    u64 commit_pos;
    u64 data_size;
    struct m_PoolFreeNode *head;    
} m_Pool;


/****************************************
 * 64-bit Pool - initializers
****************************************/

/*!
 * \brief Initializes the pool
 *
 * This function initializes the pool variables and reserves the M_POOL_DEFAULT_RESERVE bytes of memory,
 * The default memory base will be used for the reserve and for the pool allocator.
 * 
 * \param pool Pool to be initialized
 * \param data_size Size of the data that should be allocated in the pool
 */
void m_pool_begin(m_Pool *pool, u64 data_size);
/*!
 * \brief Initializes the pool with a custom reserve
 *
 * This function initializes the pool variables and reserves the specified number of bytes of memory,
 * The default memory base will be used for the reserve and for the pool allocator.
 * 
 * \param pool Pool to be initialized
 * \param reserve The amount of bytes to be reserved for the pool
 * \param data_size Size of the data that should be allocaed in the pool
 */
void m_pool_begin_reserve(m_Pool * pool, u64 reserve, u64 data_size);
/*!
 * \brief Initializes the pool with a custom memory base 
 *
 * This function initializes the pool variables and reserves the M_POOL_DEFAULT_RESERVE of memory,
 * The custom memory base will be used for the reserve and for the pool allocator.
 * 
 * \param pool Pool to be initialized
 * \param base The custom memory base
 * \param data_size Size of the data that should be allocaed in the pool
 */
void m_pool_begin_base(m_Pool *pool, m_MemoryBase *base, u64 data_size);
/*!
 * \brief Initializes the pool with a custom memory base and a custom reserve 
 *
 * This function initializes the pool variables and reserves the specified number of bytes of memory,
 * The custom memory base will be used for the reserve and for the pool allocator.
 * 
 * \param pool Pool to be initialized
 * \param base The custom memory base
 * \param reserve The amount of bytes to be reserved for the pool
 * \param data_size Size of the data that should be allocated in the pool
 */
void m_pool_begin_reserve_base(m_Pool * pool, m_MemoryBase *base, u64 reserve, u64 data_size);


/****************************************
 * 64-bit Pool - destructors
****************************************/

/*!
 * \brief Releases all the memory used by the pool 
 *
 * \warning To use the pool after this function reinitialize it with the m_pool_begin functions or use the m_pool_reset() function instead.
 * 
 * \see m_pool_reset()
 */
void m_pool_end(m_Pool *pool);

/*!
 * \brief Releases all the memory used by the pool and reinitializes it.
 *
 * The pool is useable even after this function.
 */
void m_pool_reset(m_Pool *pool);

/****************************************
 * 64-bit Pool - allocations
****************************************/

/*!
 * \brief Allocates memory inside the pool
 *
 * This funcion allocates memory in the pool and returns a pointer to it.
 * When the memory exceeds the pools capacity null is returned.
 *
 * When the pool's memory exceeds the commit_pos the M_POOL_COMMIT_BLOCK * data_size of memory is allocated.
 *
 * \param pool Pool to alocate in
 * \returns Pointer to the allocated memory
 */
VoidPtrResult m_pool_alloc(m_Pool *pool);

/*!
 * \brief Deallocates memory from the pool
 *
 * This function deallocates memory from the pool and changes the pool's free list accordingly.
 *
 * \pre ptr must be prevously allocated inside the pool
 *
 * \param pool Pool to dealocate from
 * \param ptr Pointer to the memory to be deallocated
 */
void m_pool_dealloc(m_Pool *pool, void* ptr);

/*!
 * \brief Dealocates an array of elements from the pool
 *
 * This function deallocates an array of memory from the pool and changes the pool's free list accordingly.
 *
 * \pre ptr must be prevously allocated inside the pool
 * 
 * \param pool Pool to dealocate from
 * \param ptr Pointer to the memory to be deallocated
 * \param count Number of elements to deallocate
 */
void m_pool_dealloc_count(m_Pool *pool, void* ptr, u64 count);

#endif
