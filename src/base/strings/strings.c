#include <c_base/base/strings/strings.h>
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_Strings)

struct C_String {
  ComplexBase base;
  u32 len;
  u8* chars;
  bool allocated;
};

/******************************
 * new/dest
 ******************************/
C_String* C_String_new(u8* chars, u32 len) {
  C_String* self = allocate(sizeof(C_String));
  new(self, C_String_destroy);

  self->allocated = false;
  self->chars = chars;
  self->len = len;

  return self;
}

C_String* C_String_new_view(StringView view) {
  return C_String_new(view.chars, view.len);
}

C_String* C_String_new_empty(u32 len) {
  C_String* self = allocate(sizeof(C_String));
  new(self, C_String_destroy);

  self->allocated = true;
  self->chars = allocate(len);
  self->len = len;

  mem_set(self->chars, 0, len);

  return self;
}

void C_String_destroy(void* self) {
  C_String* self_cast = self;
  if(self_cast->allocated) {
    deallocate(self_cast->chars);  
  }
}

/******************************
 * logic
 ******************************/
u8 C_String_at(C_String* self, u32 index) {
  if(index >= self->len) {
    crash(E(EG_Strings, E_OutOfBounds, SV("C_String_at -> index is outside of the string")));
  }
  
  return self->chars[index];
}

void C_String_put(C_String* self, u32 index, u8 character) {
  if(!self->allocated) {
    crash(E(EG_Strings, E_InvalidArgument, SV("C_String_set -> only allocated strings can be mutated")));
  }
  if(index >= self->len) {
    crash(E(EG_Strings, E_OutOfBounds, SV("C_String_set -> index is outside of the string")));
  }

  self->chars[index] = character;
}

C_String* C_String_substr_R(C_String* original, u32 index, u32 len) {
  if(index + len > original->len) {
    crash(E(EG_Strings, E_OutOfBounds, SV("C_String_substr -> substring would be longer than the original"))); 
  }
  
  return C_String_new(original->chars + index, len);
}

C_String* C_String_concat_R(C_String* a, C_String* b) {
  C_String* result = C_String_new_empty(a->len + b->len);

  mem_copy(result->chars, a->chars, a->len);
  mem_copy(result->chars + a->len, b->chars, b->len);

  return result;
}

/******************************
  * get/set
 ******************************/
u8* C_String_get_chars(C_String* self) {
  return self->chars;
}

u32 C_String_get_len(C_String* self) {
  return self->len;
}
