#include "c_base/base/strings/string_convert.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/strings.h>
#include <c_base/base/varargs.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_Strings)
IdImpl(IFormattable)

    struct C_String {
  ClassObject base;
  u32 len;
  ascii* chars;
  bool allocated;
};

/******************************
 * IFormattable
 ******************************/

IFormattable IFormattable_construct_format(
    C_String* (*to_str_R)(void* self),
    C_String* (*to_str_format_R)(void* self, C_String* format)) {
  IFormattable self;
  self.interface = Interface_construct(IFormattable_id);
  self.to_str_R = to_str_R;
  self.to_str_format_R = to_str_format_R;
  return self;
}

IFormattable IFormattable_construct(C_String* (*to_str_R)(void* self)) {
  return IFormattable_construct_format(to_str_R, null);
}

C_String* IFormattable_to_str_PR(void* self) {
  Ref(self);
  C_String* result;
  if (!ClassObject_contains_interface(self, IFormattable_id)) {
    result = ptr_to_str_R(self);
    goto ret;
  }

  IFormattable* i_formattable =
      (IFormattable*)ClassObject_get_interface(self, IFormattable_id);
  result = i_formattable->to_str_R(self);

ret:
  Unref(self);
  return result;
}

C_String* IFormattable_to_str_format_PR(void* self, C_String* format) {
  Ref(self);
  Ref(format);

  C_String* result;
  if (!ClassObject_contains_interface(self, IFormattable_id)) {
    result = ptr_to_str_R(self);
    goto ret;
  }

  IFormattable* i_formattable =
      (IFormattable*)ClassObject_get_interface(self, IFormattable_id);
  if (i_formattable->to_str_format_R != null) {
    result = i_formattable->to_str_format_R(self, format);
    goto ret;
  }

  result = i_formattable->to_str_R(self);

ret:
  Unref(self);
  Unref(format);
  return result;
}
/******************************
 * interface impl
 ******************************/
C_String* C_String_to_str_R(void* self) { return Ref(self); }

u32 C_String_hash(void* self) {
  C_String* self_cast = self;
  // hash by poitner to chars, that will never change
  return hash(&self_cast->chars, sizeof(void*));
}

bool C_String_equals(void* a, void* b) {
  if (a == b)
    return true;

  C_String* a_cast = a;
  C_String* b_cast = b;

  if (a_cast->len != b_cast->len)
    return false;

  return mem_equals(a_cast->chars, b_cast->chars, a_cast->len);
}

/******************************
 * new/dest
 ******************************/
static Interface* C_String_interfaces[3];
static IFormattable C_String_i_formattable = {0};
static IHashable C_String_i_hashable = {0};

static void C_String_init_interfaces(void) {
  if (!Interface_initialized((Interface*)&C_String_i_formattable)) {
    C_String_i_formattable = IFormattable_construct(C_String_to_str_R);
    C_String_i_hashable = IHashable_construct(C_String_equals, C_String_hash);

    C_String_interfaces[0] = (Interface*)&C_String_i_formattable;
    C_String_interfaces[1] = (Interface*)&C_String_i_hashable;
    C_String_interfaces[2] = null;
  }
}

C_String* C_String_new(ascii* chars, u32 len) {
  C_String_init_interfaces();
  C_String* self = allocate(sizeof(C_String));
  self->base = ClassObject_construct(C_String_destroy, C_String_interfaces);

  self->allocated = false;
  self->chars = chars;
  self->len = len;

  return self;
}

C_String* C_String_new_view(StringView view) {
  return C_String_new(view.chars, view.len);
}

C_String* C_String_new_empty(u32 len) {
  C_String_init_interfaces();
  C_String* self = allocate(sizeof(C_String));
  self->base = ClassObject_construct(C_String_destroy, C_String_interfaces);

  self->allocated = true;
  self->chars = allocate(len);
  self->len = len;

  mem_set(self->chars, 0, len);

  return self;
}

void C_String_destroy(void* self) {
  C_String* self_cast = self;
  if (self_cast->allocated) {
    deallocate(self_cast->chars);
  }
}

/******************************
 * logic
 ******************************/
ascii C_String_at(C_String* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_Strings, E_OutOfBounds,
            SV("C_String_at -> index is outside of the string")));
  }

  return self->chars[index];
}

void C_String_put(C_String* self, u32 index, ascii character) {
  if (!self->allocated) {
    crash(E(EG_Strings, E_InvalidArgument,
            SV("C_String_set -> only allocated strings can be mutated")));
  }
  if (index >= self->len) {
    crash(E(EG_Strings, E_OutOfBounds,
            SV("C_String_set -> index is outside of the string")));
  }

  self->chars[index] = character;
}

C_String* C_String_substr_R(C_String* original, u32 index, u32 len) {
  if (index + len > original->len) {
    crash(E(
        EG_Strings, E_OutOfBounds,
        SV("C_String_substr -> substring would be longer than the original")));
  }

  return C_String_new(original->chars + index, len);
}

C_String* C_String_concat_PR(C_String* string, ...) {
  Ref(string);
  C_String* result;

  C_Array* args;
  VarargsLoad(args, string);

  u32 len = string->len;
  u32 index = 0;
  C_ArrayForeach(args, { len += ((C_String*)value)->len; });

  result = C_String_new_empty(len);
  mem_copy(C_String_get_chars(result), C_String_get_chars(string), string->len);
  index += string->len;
  C_ArrayForeach(args, {
    C_String* loop_string = value;
    mem_copy(C_String_get_chars(result) + index,
             C_String_get_chars(loop_string), loop_string->len);
    index += loop_string->len;
  });

  Unref(string);
  Unref(args);
  return result;
}

/******************************
 * get/set
 ******************************/
ascii* C_String_get_chars(C_String* self) { return self->chars; }

u32 C_String_get_len(C_String* self) { return self->len; }
