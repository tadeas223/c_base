#ifndef RESULTS_H
#define RESULTS_H

#include <c_base/base/errors/Error.h>

typedef struct C_EmptyResult C_EmptyResult;

C_EmptyResult* C_EmptyResult_new_ok(void);
C_EmptyResult* C_EmptyResult_new_err(Error err);
void C_EmptyResult_destroy(void* self);

void C_EmptyResult_force(C_EmptyResult* self);

Error C_EmptyResult_get_err(C_EmptyResult* self);
bool C_EmptyResult_get_ok(C_EmptyResult* self);

#endif
