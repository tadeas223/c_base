#include "c_base/base/memory/memory.h"
#include "c_base/base/memory/memory_base.h"
#include "c_base/system.h"
#include <c_base/base/memory/allocator.h>

typedef struct AllocatorNode {
  struct AllocatorNode* next;
  u64 size;
  bool used;
} AllocatorNode;
#define AllocatorNodeAligned mem_align_forward(sizeof(AllocatorNode), MemAlign)

typedef struct {
  b8* memory;
  u64 reserve_pos;
  u64 commit_pos;
  u64 pos;
  AllocatorNode* head;
} Allocator;

static Allocator allocator = {0};

Allocator Allocator_construct(void) {
  Allocator self;

  self.pos = 0;

  MemoryResult reserve_result =
      global_memory_base->reserve(global_memory_base, AllocatorReserveSize);
  if (!reserve_result.ok) {
    crash(reserve_result.error);
  }

  self.memory = reserve_result.ptr;
  self.reserve_pos = reserve_result.size;

  MemoryResult commit_result = global_memory_base->commit(
      global_memory_base, self.memory, AllocatorCommitSize);
  if (!commit_result.ok) {
    crash(commit_result.error);
  }

  self.head = (AllocatorNode*)self.memory;
  *self.head = (AllocatorNode){.next = null, .size = commit_result.size};
  self.commit_pos = commit_result.size;
  return self;
}

void Allocator_commit(Allocator* self, u64 min_size) {
  u64 commit_size =
      (min_size > AllocatorCommitSize) ? min_size : AllocatorCommitSize;

  if (commit_size > AllocatorReserveSize) {
    crash(E(EG_Memory, E_OutOfBounds,
            SV("Allocator_commit -> allocator run out of memory")));
  }

  MemoryResult commit_result = global_memory_base->commit(
      global_memory_base, self->memory + self->commit_pos, commit_size);
  if (!commit_result.ok) {
    crash(commit_result.error);
  }

  AllocatorNode* new_node = (AllocatorNode*)(self->memory + self->commit_pos);
  new_node->next = null;
  new_node->size = commit_result.size;

  AllocatorNode* new_head = self->head;
  while (new_head->next) {
    new_head = new_head->next;
  }

  AllocatorNode* last_node = self->head;
  while (last_node->next) {
    last_node = last_node->next;
  }

  last_node->next = new_node;

  self->commit_pos += commit_result.size;
}

void* Allocator_allocate(Allocator* self, u64 size) {
  size = mem_align_forward(size, MemAlign);

  AllocatorNode* node = self->head;
  AllocatorNode** prev_node = &self->head;
  while (node) {
    // if fits two memory block
    if (node->size >=
        AllocatorNodeAligned + size + AllocatorNodeAligned + MemAlign) {
      AllocatorNode* new_node =
          (AllocatorNode*)((b8*)node + AllocatorNodeAligned + size);
      new_node->size = node->size - (AllocatorNodeAligned + size);
      new_node->used = false;
      new_node->next = node->next;

      node->size = AllocatorNodeAligned + size;
      node->next = null;
      node->used = true;

      *prev_node = new_node;

      return (b8*)node + AllocatorNodeAligned;
    } else if (node->size >= AllocatorNodeAligned + size) {
      *prev_node = node->next;

      node->used = true;
      node->next = null;

      return (b8*)node + AllocatorNodeAligned;
    }

    prev_node = &node->next;
    node = node->next;
  }

  Allocator_commit(self, size);
  return Allocator_allocate(self, size);
}

void Allocator_deallocate(Allocator* self, void* ptr) {
  AllocatorNode* loop_node = self->head;
  AllocatorNode* prev_node = null;
  AllocatorNode* ptr_node = (AllocatorNode*)((b8*)ptr - AllocatorNodeAligned);

  while (loop_node < ptr_node) {
    if (loop_node == null) {
      break;
    }

    prev_node = loop_node;
    loop_node = loop_node->next;
  }

  ptr_node->next = loop_node;
  if (prev_node == null) {
    self->head = ptr_node;
  } else {
    prev_node->next = ptr_node;
  }

  ptr_node->used = false;

  // merge with next node
  if ((AllocatorNode*)((u8*)ptr_node + ptr_node->size) == ptr_node->next) {
    ptr_node->size += ptr_node->next->size;
    ptr_node->next = ptr_node->next->next;
  }

  // merge with prev node
  if (prev_node != null &&
      (AllocatorNode*)((u8*)prev_node + prev_node->size) == ptr_node) {
    prev_node->size += ptr_node->size;
    prev_node->next = prev_node->next->next;
  }
}
void* allocate(u64 size) {
  if (allocator.head == null) {
    allocator = Allocator_construct();
  }

  return Allocator_allocate(&allocator, size);
}

void deallocate(void* ptr) { Allocator_deallocate(&allocator, ptr); }

void* reallocate(void* ptr, u64 size) {
  if (allocator.head == null) {
    allocator = Allocator_construct();
  }

  if (ptr == null) {
    return Allocator_allocate(&allocator, size);
  }

  AllocatorNode* ptr_node = (AllocatorNode*)((u8*)ptr - AllocatorNodeAligned);
  u64 new_size = (ptr_node->size - AllocatorNodeAligned >= size)
                     ? ptr_node->size - AllocatorNodeAligned
                     : size;

  void* new_ptr = Allocator_allocate(&allocator, new_size);

  mem_copy(new_ptr, ptr, ptr_node->size - AllocatorNodeAligned);

  Allocator_deallocate(&allocator, ptr);

  return new_ptr;
}

/* for debugging, because my implementation is dog shit
void* allocate(u64 size) { return malloc(size); }
void deallocate(void* ptr) { free(ptr); }
void* reallocate(void* ptr, u64 size) { return realloc(ptr, size); }
*/
