#ifndef RESULTS_H
#define RESULTS_H

#include "base/errors/errors.h"

typedef struct C_Result C_Result;
typedef struct C_EmptyResult C_EmptyResult;

/******************************
 * C_Result -> new/dest
 ******************************/
C_Result* C_Result_new_ok(void* value);
C_Result* C_Result_new_err(Error err);
void C_Result_destroy(void* self);

/******************************
 * C_Result -> logic
 ******************************/
void* C_Result_force_B(C_Result* self);
void* C_Result_force_R(C_Result* self);

/******************************
 * C_Result -> get/set
 ******************************/
bool C_Result_get_ok(C_Result* self);
Error C_Result_get_err(C_Result* self);

void* C_Result_get_value_R(C_Result* self); /* alias to C_Result_force_R */
void* C_Result_get_value_B(C_Result* self); /* alias to C_Result_force_B */

/******************************
 * C_EmptyResult -> new/dest
 ******************************/
C_EmptyResult* C_EmptyResult_new_ok(void);
C_EmptyResult* C_EmptyResult_new_err(Error err);
void C_EmptyResult_destroy(void* self);

/******************************
 * C_EmptyResult -> logic
 ******************************/
void C_EmptyResult_force(C_EmptyResult* self);

/******************************
 * C_EmptyResult -> get/set
 ******************************/
Error C_EmptyResult_get_err(C_EmptyResult* self);
bool C_EmptyResult_get_ok(C_EmptyResult* self);

#endif
