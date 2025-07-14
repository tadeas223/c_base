#ifndef TEMP_H
#define TEMP_H

#include "base/memory/objects.h"

#define CreateGlobalTemp()\
do {\
  C_Temp* t = C_Temp_new();\
  C_Temp_set_global(t);\
  unref(t);\
} while(0)

#define ResetGlobalTemp() C_Temp_reset_global()

typedef struct C_Temp C_Temp;

/******************************
 * new/dest
 ******************************/
C_Temp* C_Temp_new(void);
void C_Temp_destroy(void* self);

/******************************
 * logic
 ******************************/
void* C_Temp_put_B(C_Temp* self, void* obj);
void C_Temp_clear(C_Temp* self);

void* C_Temp_global_put_B(void* obj);
void C_Temp_global_clear(void);

void* temp(void* obj); /* alias to C_Temp_global_put_B */

/******************************
 * get/set
 ******************************/
void C_Temp_set_global(C_Temp* temp);
void C_Temp_reset_global(void);
C_Temp* C_Temp_get_global(void);

#endif
