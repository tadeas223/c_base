#ifndef C_RESULT_H
#define C_RESULT_H

#include "c_base/base/errors/Error.h"

typedef struct C_Result C_Result;

C_Result* C_Result_new_ok_P(void* value);
C_Result* C_Result_new_err(Error err);
void C_Result_destroy(void* self);

void* C_Result_force_B(C_Result* self);
void* C_Result_force_R(C_Result* self);

bool C_Result_get_ok(C_Result* self);
Error C_Result_get_err(C_Result* self);

void* C_Result_get_value_R(C_Result* self); /* alias to C_Result_force_R */
void* C_Result_get_value_B(C_Result* self); /* alias to C_Result_force_B */

#endif
