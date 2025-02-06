/************************************************************
 * \file os_io.h
 * \brief OS dependent memory allocation.
************************************************************/
#ifndef OS_MEM_H
#define OS_MEM_H

#include "base/mem.h"
#include "base/types.h"

/*!
 * \brief Reserves memory but does not map it to real ram
 */
void* os_memory_reserve(u64 size);

/*!
 * \brief Maps reserved memory to real ram 
*/
void os_memory_commit(void* ptr, u64 size);

/*!
 * \brief Frees memory from real ram but does not release it 
*/
void os_memory_decommit(void* ptr, u64 size);

/*!
 * \brief Frees memory from virtual ram and real ram
*/
void os_memory_release(void* ptr, u64 size);

/*!
 * \brief m_MemoryBase based on top of os dependent allcoations
 */
m_MemoryBase* os_memory_base();
#endif
