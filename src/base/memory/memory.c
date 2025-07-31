#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/memory.h>

GenericValImpl_ErrorCode(EG_Memory)

void mem_copy(void* dest, void* src, u64 size) {
  u8* u8_src = src;
  u8* u8_dest = dest;
  u32 i;

  if (u8_dest > u8_src && u8_dest < u8_src + size) {
    i = size;
    while (i > 0) {
      u8_dest[i] = u8_src[i];
    }
  } else {
    for (i = 0; i < size; i++) {
      u8_dest[i] = u8_src[i];
    }
  }
}

void mem_set(void* dest, u8 byte, u64 size) {
  u8* u8_dest = dest;
  while (size--) {
    u8_dest[size] = byte;
  }
}

bool mem_equals(void* a, void* b, u64 size) {
  u8* u8_a = a;
  u8* u8_b = b;

  while (size--) {
    if (u8_a[size] != u8_b[size]) {
      return false;
    }
  }
  return true;
}

u64 mem_align_forward(u64 value, u64 align) {
  u64 modulo = value % align;

  if (modulo == 0) {
    return value;
  } else {
    return value + (align - modulo);
  }
}
