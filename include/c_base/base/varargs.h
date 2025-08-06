#ifndef VARARGS_H
#define VARARGS_H

#include <c_base/base/strings/strings.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_List.h>
#include <stdarg.h>

#define ArgsEnd (u64)0xDEADAF

#define VarargsLoad(args, param)                                               \
  do {                                                                         \
    va_list __args;                                                            \
    va_list __args_copy;                                                       \
    va_start(__args, param);                                                   \
    va_copy(__args_copy, __args);                                              \
                                                                               \
    C_List* args_list = C_List_new();                                          \
    C_List_push_P(args_list, param);                                           \
    void* value = va_arg(__args_copy, void*);                                  \
    while ((u64)value != ArgsEnd) {                                            \
      C_List_push_P(args_list, value);                                         \
      value = va_arg(__args_copy, void*);                                      \
    };                                                                         \
                                                                               \
    args = C_List_to_array_PR(Pass(args_list));                                \
                                                                               \
    va_end(__args);                                                            \
    va_end(__args_copy);                                                       \
  } while (0)

#define ArgsLoad(args, argc, argv)                                             \
  do {                                                                         \
    args = C_Array_new(argc);                                                  \
    C_ArrayForeach(args, {                                                     \
      C_Array_put_P(args, iter,                                                \
                    Pass(C_String_new(argv[iter], cstr_len(argv[iter]))));     \
    });                                                                        \
  } while (0)

#endif
