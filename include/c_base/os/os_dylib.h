#ifndef OS_DYLIB_H
#define OS_DYLIB_H

#include <c_base/base/errors/C_Result.h>
#include <c_base/base/strings/strings.h>

typedef struct C_DynamicLibrary C_DynamicLibrary;

C_Result* /* C_DynamicLibrary* */ C_DynamicLibrary_new_P(C_String* path);
C_EmptyResult* C_DynamicLibrary_close(C_DynamicLibrary* self);
void C_DynamicLibrary_destroy(void* self);

C_Result* /* C_Ptr* */ C_DynamicLibrary_load_P(
  C_DynamicLibrary* self, C_String* name);

#endif
