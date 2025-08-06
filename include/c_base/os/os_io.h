#ifndef OS_IO_H
#define OS_IO_H

#include <c_base/base/errors/results.h>
#include <c_base/base/strings/strings.h>

GenericVal_ErrorCode(EG_OS_IO)

C_Result* /* u32 */ console_read_chars_R(ascii* chars, u32 len);
C_Result* /* u32 */ console_write_chars_R(ascii* chars, u32 len);

void console_write_P(void* obj, ...);
void console_write_ln_P(void* obj, ...);

void console_write_single_P(void* obj);
void console_write_single_ln_P(void* obj);

C_String* console_read_until_R(ascii character);
C_String* console_read_ln_R(void);

C_EmptyResult* console_set_raw_node(bool value);

/******************************
 * files
 ******************************/
typedef struct C_File C_File;

typedef enum {
  FILE_R,
  FILE_W,
  FILE_RW,
  FILE_A,
  FILE_AR,
} FileMode;

C_Result* /* C_File* */ C_File_new_open_P(C_String* path, FileMode mode);
C_Result* /* C_File* */ C_File_new_create_P(C_String* path, FileMode mode);

C_EmptyResult* C_File_close(C_File* self);
void C_File_destroy(void* self);

C_Result* /* u32 */ C_File_read_chars_R(C_File* self, ascii* chars, u32 len);
C_Result* /* u32 */ C_File_write_chars_R(C_File* self, ascii* chars, u32 len);

void C_File_write_P(C_File* self, void* obj, ...);
void C_File_write_ln_P(C_File* self, void* obj, ...);

void C_File_write_single_P(C_File* self, void* obj);
void C_File_write_ln_single_P(C_File* self, void* obj);

C_String* C_File_read_until_R(ascii character);
C_String* C_File_read_ln_R(void);

#endif
