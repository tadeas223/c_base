/*!**********************************************************
 * \file  mem.h
 * \brief basic memory functions and structs
 *
 * ### contains:
 * - lowest level memory abstraction (\ref MemoryBase)
 * - global MemoryBase variable
 * - memory helper functions
 *
 ***********************************************************/
#ifndef MEM_H
#define MEM_H

#include "base/errors.h"
#include "base/types.h"

/*!
 * \brief errors related to memory
 * 
 * errors related to:
 * - memory allocations
 * - memory access
 */
Extern_ErrorGroup(EG_Memory);

/*!
 * \brief default alignment of memory
 *
 * represents the correct alignment of memory
 * for the current platform
 *
 * all allocators should allocate with blocks
 * of this size
 *
 */
#define MEM_ALIGN sizeof(void*)


// forward declaration of MemoryBase
typedef struct MemoryBase MemoryBase;

/*!
 * \brief function reserving memory
 * 
 * virtual method for \ref MemoryBase
 *
 * ### common behaviour:
 * - reserves space in virual memory
 * - does not map to physical ram
 * - the space is unuseable until \ref MemoryBaseCommit is called 
 * - returns a generated address of the reserved memory
 * - if reservation fails error is returned
 * - the error code and group is defined in the specific implementation of this function
 *
 * \param self pointer to \ref MemoryBase
 * \param size size of the desired memory space
 *
 * \return result with pointer to the reserved memory or error
 */
typedef Result(ptr) MemoryBaseReserveFunc(MemoryBase *self, u64 size);

/*!
 * \brief function releasing memory
 *
 * virtual method for \ref MemoryBase
 *
 * ### common behaviour:
 * - releases memory starting from ptr up to ptr + size
 * - accessing the memory after release will cause bad things to happen
 * - on error returns a result with result.ok = false
 * - the returned error is defined in specific implementation of this function
 *
 * \param  self pointer to \ref MemoryBase
 * \param  ptr  pointer to the start of the memory block
 * \param  size size of the memory block to release
 * 
 * \return result with a possible error
 */
typedef Result(none) MemoryBaseReleaseFunc(MemoryBase *self, void *ptr, u64 size);

/*!
 * \brief function commiting memory
 *
 * virtual method for \ref MemoryBase
 * 
 * should be used only for memory that was reserved earlier
 *
 * ### common behaviour:
 * - commits memory starting from ptr up to ptr + size
 * - maps the reserved memory block to physical ram
 * - makes the memory block useable
 * - on error returns a result with result.ok = false
 * - the returned error is defined in specific implementation of this function
 * 
 * \param  self pointer to \ref MemoryBase
 * \param  ptr  pointer to the start of the memory block
 * \param  size size of the memory block to commit 
 * 
 * \return result with a possible error
 */
typedef Result(none) MemoryBaseCommitFunc(MemoryBase *self, void *ptr, u64 size);

/*!
 * \brief function decommiting memory
 *
 * virtual method for \ref MemoryBase
 *
 * should be used only for memory that was commited earlier
 *
 * ### common behaviour:
 * - decommits memory starting from ptr up to ptr + size
 * - unmaps the reserved memory block from physical ram
 * - makes the memory block unuseable
 * - on error returns a result with result.ok = false
 * - the returned error is defined in specific implementation of this function
 *
 * \param  self pointer to \ref MemoryBase
 * \param  ptr  pointer to the start of the memory block
 * \param  size size of the memory block to decommit 
 * 
 * \return result with a possible error
 */
typedef Result(none) MemoryBaseDecommitFunc(MemoryBase *self, void *ptr, u64 size);

/*!
 * \brief abstraction for simple memory functions
 *
 * this struct can be thought of as a object with virtual methods
 * related to memory allocation
 *
 * it serves as a interface for other functions or higher level allocators
 * to actually allocate memory
 *
 * it should not be used directly for memory allocations although it can,
 * but other allocators should use it inside
 */
struct MemoryBase {
    /*!
     * reserving memory function 
     * \see MemoryBaseReserveFunc
     */
    MemoryBaseReserveFunc *reserve;
    /*!
     * releasing memory function 
     * \see MemoryBaseReleaseFunc
     */
    MemoryBaseReleaseFunc *release;
    /*!
     * committing memory function 
     * \see MemoryBaseCommitFunc
     */
    MemoryBaseCommitFunc *commit;
    /*!
     * decommiting memory function 
     * \see MemoryBaseDecommitFunc
     */
    MemoryBaseDecommitFunc *decommit;
    /*! place to store higher level allocator data */
    void *ctx;
};

/*!
 * \brief sets a global \ref MemoryBase
 *
 * the global variable can be accesed from everywhere
 * to get the current \ref MemoryBase implemetation the user had set
 *
 * allcators may set its internal \ref MemoryBase with this variable by
 * the \ref global_memory_base_get() function
 * 
 * \param base \ref MemoryBase pointer
 */
/* tested */ void global_memory_base_set(MemoryBase *base);

/*!
 * \brief   gets the global \ref MemoryBase
 * 
 * \warning this function will crash the program if the 
 *          \ref global_memory_base_set() function was not called before
 * \return  pointer to the global \ref MemoryBase
 */
/* tested */ MemoryBase* global_memory_base_get();

/*!
 * \brief rounds up a value to the nearest align 
 * 
 * \code
 * u32 value = mem_align_forward(4, 8); // value = 8
 * \endcode
 *
 * \param val   value to be rounded
 * \param align value to align with
 * 
 * \return value that is divisible by align
 */
/* tested */ u64 mem_align_forward(u64 val, u64 align);

/*!
 * \brief rounds down a value to the nearest align
 * 
 * \code
 * u32 value = mem_align_forward(6, 8); \\ value = 0
 * u32 value2 = mem_align_forward(10, 8); \\ value2 = 8
 * \endcode
 *
 * \param val   value to be rounded
 * \param align value to align with
 * 
 * \return value that is divisible by align
 */
/* tested */ u64 mem_align_backward(u64 val, u64 align);

/*!
 * \brief coppies memory from one place to another
 *
 * coppies the size bytes from src to dest
 *
 * if src > dest memory is coppied from left to right
 * if dest > src memory is coppied from right to left
 * 
 * **be careful with this function because when used incorrectly
 * it can cause segmentation faults**
 *
 * \param dest place where the data will be coppied to
 * \param src  place from where the data will be coppied
 * \param size ammount of bytes to copy
 */
/* tested */ void mem_copy(void* dest, void* src, u64 size);

#endif
