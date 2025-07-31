#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/darray.h"
#include "c_base/os/os_io.h"

int main(void) {
  C_DArray* darray = C_DArray_new();

  C_DArray_push_P(darray, Pass(C_Handle_u32_new(1)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(2)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(3)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(4)));
  C_DArray_push_front_P(darray, Pass(C_Handle_u32_new(5)));

  C_Darray_add_P(darray, 2, Pass(C_Handle_u32_new(300)));
  Unref(C_Darray_remove_R(darray, 2));

  C_DArrayForeach(darray, { console_write_single_ln_P(value); });

  Unref(darray);
}
