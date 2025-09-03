#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/string_convert.h>
#include <c_base/base/strings/strings.h>
#include <c_base/base/varargs.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_List.h>
#include <c_base/env.h>
#include <c_base/os/os_io.h>
#include <c_base/os/os_threads.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_OS_IO)

static Mutex write_mutex = MutexConstructStatic;

void console_write_P(void* obj, ...) {
  Mutex_lock(&write_mutex);
  C_Array* args;
  VarargsLoad(args, obj);

  C_ArrayForeach(args, {
    C_String* text = IFormattable_to_str_PR(value);

    C_Result* result =
      console_write_chars_R(C_String_get_chars(text), C_String_get_len(text));
    if (C_Handle_u32_get_value(C_Result_force_B(result)) !=
        C_String_get_len(text)) {
      crash(E(EG_OS_IO, E_Unspecified,
        SV("console_write -> invalid number or characters written")));
    }
    Unref(result);
    Unref(text);
  });

  Unref(args);
  Mutex_unlock(&write_mutex);
}

void console_write_ln_P(void* obj, ...) {
  C_Array* args;
  VarargsLoad(args, obj);

  C_List* list = C_List_new();
  C_ArrayForeach(args, { C_List_push_P(list, value); });
  C_List_push_P(list, PS("\n"));

  C_String* join = C_String_join_PR(Pass(C_List_to_array_PR(list)));

  console_write_P(join, ArgsEnd);

  Unref(join);
  Unref(list);
  Unref(args);
}

void console_write_single_P(void* obj) {
  Ref(obj);
  console_write_P(obj, ArgsEnd);
  Unref(obj);
}

void console_write_single_ln_P(void* obj) {
  Ref(obj);
  console_write_ln_P(obj, ArgsEnd);
  Unref(obj);
}

C_String* console_read_until_R(ascii character) {
#define READ_LEN 1024
  ascii buffer[READ_LEN];
  ascii* chars = null;
  u32 len = 0;
  bool end = false;

  while (!end) {
    C_Result* read_result = console_read_chars_R(buffer, READ_LEN);
    u32 read_len = C_Handle_u32_get_value(C_Result_force_B(read_result));
    Unref(read_result);

    for (u32 i = 0; i < read_len; i++) {
      if (buffer[i] == character) {
        end = true;
        read_len = i;
        break;
      }
    }

    len += read_len;
    chars = reallocate(chars, len);
    mem_copy(chars + len - read_len, buffer, read_len);
  }

  return C_String_new(chars, len);
#undef READ_LEN
}

C_String* console_read_ln_R(void) { return console_read_until_R('\n'); }
