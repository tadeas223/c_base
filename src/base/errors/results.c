#include <c_base/base/errors/results.h>
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/system.h>

struct C_Result {
  ComplexBase base; 
  void* value;
  Error err;
  bool ok;
};

struct C_EmptyResult {
  ComplexBase base; 
  Error err;
  bool ok;
};

/******************************
 * C_Result -> new/dest
 ******************************/
C_Result* C_Result_new_ok(void* value) {
  C_Result* self = allocate(sizeof(C_Result));
  new(self, C_Result_destroy);
  
  self->ok = true;
  self->value = ref(value);
  SetZero(&self->err);
  
  return self;
}

C_Result* C_Result_new_err(Error err) {
  C_Result* self = allocate(sizeof(C_Result));
  new(self, C_Result_destroy);

  self->ok = false;
  self->err = err;
  self->value = null;

  return self;
}

void C_Result_destroy(void* self) {
  C_Result* self_cast = self;
  unref(self_cast->value);
}

/******************************
 * C_Result -> logic
 ******************************/
static void* __C_Result_force(C_Result* self) {
  if(!self->ok) {
    crash(self->err);
  }

  return ref(self->value);
}

/******************************
 * C_Result -> get/set
 ******************************/
bool C_Result_get_ok(C_Result* self) {
  return self->ok;
}

Error C_Result_get_err(C_Result* self) {
  if(self->ok) {
    crash(E(EG_Unspecified, E_Unspecified, SV("C_Result_get_err -> result is ok, cannot get an error")));
  }

  return self->err;
}

static void* __C_Result_get_value(C_Result* self) {
  return __C_Result_force(self);
}

// {{{ C_Result -> _R _B wrappers

/******************************
 * C_Result -> _R _B wrappers
 ******************************/
void* C_Result_force_B(C_Result* self) {
  void* result = __C_Result_force(self);
  unref(result);
  return result;
}

void* C_Result_force_R(C_Result* self) {
  void* result = __C_Result_force(self);
  return result;
}

void* C_Result_get_value_R(C_Result* self) {
  void* result = __C_Result_get_value(self);
  unref(result);
  return result;
}

void* C_Result_get_value_B(C_Result* self) {
  void* result = __C_Result_get_value(self);
  return result;
}
// }}}

/******************************
 * C_EmptyResult -> new/dest
 ******************************/
C_EmptyResult* C_EmptyResult_new_ok(void) {
  C_EmptyResult* self = allocate(sizeof(C_EmptyResult));
  new(self, C_EmptyResult_destroy);

  self->ok = true;
  SetZero(&self->err);
  
  return self;
}

C_EmptyResult* C_EmptyResult_new_err(Error err) {
  C_EmptyResult* self = allocate(sizeof(C_EmptyResult));
  new(self, C_EmptyResult_destroy);

  self->ok = false;
  self->err = err;

  return self;
}

void C_EmptyResult_destroy(void* self) {(void)self;}

/******************************
 * C_EmptyResult -> logic
 ******************************/
void C_EmptyResult_force(C_EmptyResult* self) {
  if(!self->ok) {
    crash(self->err); 
  }
}

/******************************
 * C_EmptyResult -> get/set
 ******************************/
Error C_EmptyResult_get_err(C_EmptyResult* self) {
  if(self->ok) {
    crash(E(EG_Unspecified, E_Unspecified, SV("C_EmptyResult_get_err -> result is ok, cannot get an error")));
  }

  return self->err;
}

bool C_EmptyResult_get_ok(C_EmptyResult* self) {
  return self->ok;
}

