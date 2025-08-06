#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/format.h>
#include <c_base/base/strings/strings.h>
#include <c_base/ds/C_Array.h>

C_String* format_get_value_PR(C_String* format, C_String* key) {
  Ref(key);
  C_Array* split1 = C_String_split_R(format, ';');

  C_String* result;

  C_ArrayForeach(split1, {
    C_Array* split2 = C_String_split_R(value, '=');
    if (C_String_equals(C_Array_at_B(split2, 0), key)) {
      result = C_Array_at_R(split2, 1);
    }

    Unref(split2);
  });

  Unref(split1);
  Unref(key);
  return result;
}
