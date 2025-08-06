#include "c_base/base/strings/string_convert.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/array.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/errors/results.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/strings.h>
#include <c_base/env.h>
#include <c_base/os/os_io.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_OS_IO)

#if defined(PLATFORM_LINUX)
#include "impl/linux/linux_io.c"
#else
#error "os_io.c -> unsupported platform"
#endif

void console_write_P(void* obj, ...) {
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
}

void console_write_ln_P(void* obj, ...) {
  C_Array* args;
  VarargsLoad(args, obj);

  C_ArrayForeach(args, { console_write_P(value, ArgsEnd); });

  console_write_P(PS("\n"), ArgsEnd);

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
