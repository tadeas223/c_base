#include <c_base/base/memory/memory.h>
#include <c_base/os/os_mem.h>

#include <sys/mman.h>
#include <unistd.h>

MemoryResult os_mem_reserve(MemoryBase* self, u64 size) {
  (void)self;
  u64 page_size = sysconf(_SC_PAGESIZE);
  page_size = mem_align_forward(size, page_size);
  void* ptr = mmap(null, page_size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (ptr == MAP_FAILED) {
    return (MemoryResult){.ok = false,
                          .ptr = null,
                          .error = E(EG_Memory, E_Unspecified,
                                     SV("os_mem_reserve -> reserve failed"))};
  }

  return (MemoryResult){
      .ok = true, .error = {0}, .ptr = ptr, .size = page_size};
}

MemoryResult os_mem_commit(MemoryBase* self, void* ptr, u64 size) {
  (void)self;
  u64 page_size = sysconf(_SC_PAGESIZE);
  page_size = mem_align_forward(size, page_size);

  int result = mprotect(ptr, page_size, PROT_READ | PROT_WRITE);
  if (result < 0) {
    return (MemoryResult){.ok = false,
                          .ptr = null,
                          .error = E(EG_Memory, E_Unspecified,
                                     SV("os_mem_commit -> commit failed"))};
  }

  return (MemoryResult){
      .ok = true, .error = {0}, .ptr = null, .size = page_size};
}

MemoryResult os_mem_decommit(MemoryBase* self, void* ptr, u64 size) {
  (void)self;
  u64 page_size = sysconf(_SC_PAGESIZE);
  page_size = mem_align_forward(size, page_size);

  int result = mprotect(ptr, page_size, PROT_NONE);
  if (result < 0) {
    return (MemoryResult){.ok = false,
                          .ptr = null,
                          .error = E(EG_Memory, E_Unspecified,
                                     SV("os_mem_decommit -> decommit failed"))};
  }

  return (MemoryResult){
      .ok = true, .error = {0}, .ptr = null, .size = page_size};
}

MemoryResult os_mem_release(MemoryBase* self, void* ptr, u64 size) {
  (void)self;
  u64 page_size = sysconf(_SC_PAGESIZE);
  page_size = mem_align_forward(size, page_size);

  int result = munmap(ptr, page_size);
  if (result < 0) {
    return (MemoryResult){.ok = false,
                          .ptr = null,
                          .error = E(EG_Memory, E_Unspecified,
                                     SV("os_mem_release -> release failed"))};
  }

  return (MemoryResult){
      .ok = true, .error = {0}, .ptr = null, .size = page_size};
}
