#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/types.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_DArray.h"
#include "c_base/ds/C_HashTable.h"
#include "c_base/os/os_io.h"

int main(void) {
  C_HashTable* table = C_HashTable_new();

  C_HashTable_put_P(table, PS("a"), Pass(C_Handle_u32_new(1)));
  C_HashTable_put_P(table, PS("b"), Pass(C_Handle_u32_new(2)));
  C_HashTable_put_P(table, PS("c"), Pass(C_Handle_u32_new(3)));
  C_HashTable_put_P(table, PS("d"), Pass(C_Handle_u32_new(4)));

  C_String* a = console_read_ln_R();

  console_write_single_ln_P(a);
  C_Handle_u32* n = C_HashTable_at_PB(table, a);

  console_write_single_ln_P(n);

  Unref(a);
  Unref(table);
  return 0;
}
