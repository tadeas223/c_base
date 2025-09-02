#ifndef OS_ATOMIC_H
#define OS_ATOMIC_H

#include "c_base/base/types.h"

bool os_atomic_u32_compare_exchange(u32* ptr, u32* expected, u32 desired);
void os_atomic_u32_store(u32* ptr, u32 val);
u32 os_atomic_u32_load(u32* ptr);

#endif
