#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/strings.h"
#include "c_base/ds/C_HashTable.h"
#include "c_base/os/os_io.h"

int main(void) {
  C_HashTable* table = C_HashTable_new();

  C_HashTable_put_P(table, Pass(C_Handle_u32_new(10)), PS("ten"));
  C_HashTable_put_P(table, Pass(C_Handle_u32_new(11)), PS("eleven"));
  C_HashTable_put_P(table, Pass(C_Handle_u32_new(1)), PS("one"));

  C_String* format = C_HashTable_to_str_R(table);
  console_write_single_ln_P(Pass(format));

  Unref(table);
  return 0;
}
