#include <c_base/base/errors/errors.h>
#include <c_base/base/errors/results.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/system.h>

struct C_EmptyResult {
  ClassObject base;
  Error err;
  bool ok;
};

/******************************
 * C_EmptyResult -> new/dest
 ******************************/
C_EmptyResult* C_EmptyResult_new_ok(void) {
  C_EmptyResult* self = allocate(sizeof(C_EmptyResult));
  self->base = ClassObject_construct(C_EmptyResult_destroy, null);

  self->ok = true;
  SetZero(&self->err);

  return self;
}

C_EmptyResult* C_EmptyResult_new_err(Error err) {
  C_EmptyResult* self = allocate(sizeof(C_EmptyResult));
  self->base = ClassObject_construct(C_EmptyResult_destroy, null);

  self->ok = false;
  self->err = err;

  return self;
}

void C_EmptyResult_destroy(void* self) { (void)self; }

/******************************
 * C_EmptyResult -> logic
 ******************************/
void C_EmptyResult_force(C_EmptyResult* self) {
  if (!self->ok) {
    crash(self->err);
  }
}

/******************************
 * C_EmptyResult -> get/set
 ******************************/
Error C_EmptyResult_get_err(C_EmptyResult* self) {
  if (self->ok) {
    crash(E(EG_Unspecified, E_Unspecified,
      SV("C_EmptyResult_get_err -> result is ok, cannot get an error")));
  }

  return self->err;
}

bool C_EmptyResult_get_ok(C_EmptyResult* self) { return self->ok; }
