#include "c_base/base/errors/errors.h"
#include "c_base/base/errors/results.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/strings.h"
#include "c_base/os/os_dylib.h"

#include <dlfcn.h>

GenericValImpl_ErrorCode(EG_OSDynLib)

struct C_DynamicLibrary {
  ClassObject base;

  void* handle;
  bool closed;
};

C_Result* /* C_DynamicLibrary* */ C_DynamicLibrary_new_P(C_String* path) {
  Ref(path);
  C_Result* result;
  C_Ptr* cstr = C_String_to_cstr(path);
  void* handle = dlopen(C_Ptr_get_ptr(cstr), RTLD_NOW);
  if (handle == null) {
    result = C_Result_new_err(
        E(EG_OSDynLib, E_Unspecified,
          SV("C_DynamicLibrary_new -> failed to load dynamic library")));
    goto ret;
  }

  C_DynamicLibrary* self = allocate(sizeof(C_DynamicLibrary));
  self->base = ClassObject_construct(C_DynamicLibrary_destroy, null);

  self->handle = handle;
  self->closed = false;

  result = C_Result_new_ok_P(self);
ret:
  Unref(cstr);
  Unref(path);
  return result;
}

C_EmptyResult* C_DynamicLibrary_close(C_DynamicLibrary* self) {
  if (self->closed) {
    return C_EmptyResult_new_ok();
  }

  int close_result = dlclose(self->handle);
  if (close_result < 0) {
    return C_EmptyResult_new_err(
        E(EG_OSDynLib, E_Unspecified,
          SV("C_DynamicLibrary_close -> failed to close dynamic library")));
  }
  self->closed = true;

  return C_EmptyResult_new_ok();
}

void C_DynamicLibrary_destroy(void* self) {
  C_DynamicLibrary* self_cast = self;

  C_EmptyResult* close_result = C_DynamicLibrary_close(self_cast);
  C_EmptyResult_force(close_result);
  Unref(close_result);
}
C_Result* /* void* */ C_DynamicLibrary_load_P(C_DynamicLibrary* self,
                                              C_String* name) {
  Ref(name);
  C_Result* result;
  C_Ptr* cstr = C_String_to_cstr(name);
  void* result_handle = dlsym(self->handle, C_Ptr_get_ptr(cstr));
  if (!result_handle) {
    result = C_Result_new_err(
        E(EG_OSDynLib, E_Unspecified,
          SV("C_DynamicLibrary_load -> failed to load specified value")));
    goto ret;
  }

  result = C_Result_new_ok_P(Pass(C_Ptr_new(result_handle)));

ret:
  Unref(cstr);
  Unref(name);
  return result;
}
