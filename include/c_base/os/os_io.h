#ifndef OS_IO_H
#define OS_IO_H

#include "base/errors/results.h"
#include "base/strings/strings.h"

GenericVal_ErrorCode(EG_OS_IO)

C_Result* /* u32 */ console_read_chars_R(void* chars, u32 len);
C_Result* /* u32 */ console_write_chars_R(void* chars, u32 len);

void console_write(C_String* text);
void console_write_ln(C_String* text);

C_String* console_read_until_R(u8 character);
C_String* console_read_ln_R(void);

#endif
