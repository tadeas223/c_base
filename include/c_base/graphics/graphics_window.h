#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "c_base/base/errors/results.h"
#include "c_base/base/strings/strings.h"

typedef struct C_Window C_Window;

C_EmptyResult* graphics_start(void);
void graphics_end(void);

C_Window* C_Window_new(u32 width, u32 height);
void C_Window_destroy(void* self);

void C_Window_show(C_Window* self);

#endif
