#ifndef VARARGS_H
#define VARARGS_H

#include "c_base/ds/array.h"
#include "c_base/ds/list.h"
#include <stdarg.h>

#define VarargsLoad(args, param)\
do {\
  va_list __args;\
  va_list __args_copy;\
  va_start(__args, param);\
  va_copy(__args_copy, __args);\
  \
  C_List* args_list = C_List_new();\
  void* value = va_arg(__args_copy, void*);\
  while(value) {\
    C_List_push_P(args_list, value);\
    value = va_arg(__args_copy, void*);\
  };\
  \
  args = C_List_to_array_PR(Pass(args_list));\
  \
  va_end(__args);\
  va_end(__args_copy);\
} while(0);


#endif
