#include "base/memory/allocator.h"
#include <stdlib.h>

void* allocate(u64 size) {
  return malloc(size);
}

void deallocate(void* ptr) {
  free(ptr);
}

void* reallocate(void* ptr, u64 size) {
  return realloc(ptr, size);
}
