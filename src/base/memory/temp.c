#include <c_base/base/memory/temp.h>
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/objects.h>
#include <c_base/ds/list.h>
#include <c_base/system.h>

static C_Temp* temp_global = null;

struct C_Temp {
  ComplexBase base;
  C_List* objects;
};

/******************************
 * new/dest
 ******************************/
C_Temp* C_Temp_new(void) {
  C_Temp* self = allocate(sizeof(C_Temp));
  new(self, C_Temp_destroy);

  self->objects = C_List_new();
  
  return self;
}

void C_Temp_destroy(void* self) {
  C_Temp* self_cast = self;
  C_Temp_clear(self);
  unref(self_cast->objects);
}

/******************************
 * logic
 ******************************/
void* C_Temp_put_B(C_Temp* self, void* obj) {
  ref(obj);
  C_List_push(self->objects, obj);
  return obj;
}

void C_Temp_clear(C_Temp* self) {
  C_List_Foreach(self->objects, {
    unref(value);
  });

  C_List_clear(self->objects);
}

void* C_Temp_global_put_B(void* obj) {
  if(temp_global == null) {
    crash(E(EG_Unspecified, E_InvalidPointer, SV("C_Temp_global_put_B -> global temp is not set")));
  }

  return C_Temp_put_B(temp_global, obj);
}

void C_Temp_global_clear(void) {
  if(temp_global == null) {
    crash(E(EG_Unspecified, E_InvalidPointer, SV("C_Temp_global_clear -> global temp is not set")));
  }
  
  C_Temp_clear(temp_global);
}

void* temp(void* obj) {
  return C_Temp_global_put_B(obj);
}

/******************************
 * get/set
 ******************************/
void C_Temp_reset_global(void) {
  unref(temp_global);
  temp_global = null;
}

void C_Temp_set_global(C_Temp* temp) {
  ref(temp);
  unref(temp_global);
  temp_global = temp;
}

C_Temp* C_Temp_get_global(void) {
  if(temp_global == null) {
    crash(E(EG_Unspecified, E_InvalidPointer, SV("C_Temp_get_global -> global temp is not set")));
  }

  return temp_global;

}
