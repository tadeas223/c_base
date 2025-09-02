#include "c_base/os/os_threads.h"
#include "c_base/system.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/strings.h>
#include <c_base/os/os_io.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static struct termios default_console_attribures = {0};
static bool default_console_attribures_set = false;

C_Result* /* u32 */ console_read_chars_R(ascii* chars, u32 len) {
  C_Result* result;
  int read_result = read(0, chars, len);
  if (read_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified,
      SV("console_read_chars -> failed to read from console")));
    goto ret;
  } else {
    C_Handle_u32* handle = C_Handle_u32_new(read_result);
    result = C_Result_new_ok_P(handle);
    Unref(handle);

    goto ret;
  }

ret:
  return result;
}

C_Result* /* u32 */ console_write_chars_R(ascii* chars, u32 len) {
  int write_result = write(1, chars, len);
  C_Result* result;
  if (write_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified,
      SV("console_write_chars -> failed to write to console")));
    goto ret;
  } else {
    C_Handle_u32* handle = C_Handle_u32_new(write_result);
    result = C_Result_new_ok_P(handle);
    Unref(handle);

    goto ret;
  }

ret:
  return result;
}

C_EmptyResult* console_set_raw_node(bool value) {
  C_EmptyResult* result;
  if (!default_console_attribures_set) {
    int attr_result = tcgetattr(0, &default_console_attribures);
    if (attr_result < 0) {
      result = C_EmptyResult_new_err(E(EG_OS_IO, E_Unspecified,
        SV("console_set_raw_mode -> failed to get terminal attributes")));
      goto ret;
    }
  }

  struct termios new_attrs = default_console_attribures;
  if (value) {

    new_attrs = default_console_attribures;
    new_attrs.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    new_attrs.c_oflag &= ~(OPOST);

    new_attrs.c_cflag |= (CS8);

    new_attrs.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    new_attrs.c_cc[VMIN] = 5;
    new_attrs.c_cc[VTIME] = 8;
    new_attrs.c_cc[VMIN] = 0;
    new_attrs.c_cc[VTIME] = 0;
    new_attrs.c_cc[VMIN] = 2;
    new_attrs.c_cc[VTIME] = 0;
    new_attrs.c_cc[VMIN] = 0;
    new_attrs.c_cc[VTIME] = 8;
  }

  int set_result = tcsetattr(0, TCSANOW, &new_attrs);
  if (set_result < 0) {
    result = C_EmptyResult_new_err(E(EG_OS_IO, E_Unspecified,
      SV("console_set_raw_mode -> failed to set new terminal attributes")));
    goto ret;
  }

  result = C_EmptyResult_new_ok();

ret:
  return result;
}

/******************************
 * files
 ******************************/
struct C_File {
  ClassObject base;
  int descriptor;
  bool closed;
};

static int file_flags(FileMode mode) {
  switch (mode) {
  case FILE_R:
    return O_RDONLY;
  case FILE_W:
    return O_WRONLY;
  case FILE_RW:
    return O_RDWR;
  case FILE_A:
    return O_APPEND | O_WRONLY;
  case FILE_AR:
    return O_APPEND | O_RDWR;

  default:
    return 0;
  }
}

C_Result* C_File_new_open_P(C_String* path, FileMode mode) {
  Ref(path);
  C_Result* result;

  C_Ptr* cstr = C_String_to_cstr(path);
  int descriptor = open(C_Ptr_get_ptr(cstr), file_flags(mode));
  Unref(cstr);

  if (descriptor < 0) {
    result = C_Result_new_err(E(
      EG_OS_IO, E_Unspecified, SV("C_File_new_open_P -> failed to open file")));
    goto ret;
  }

  C_File* self = allocate(sizeof(C_File));
  self->base = ClassObject_construct(C_File_destroy, null);

  self->descriptor = descriptor;
  self->closed = false;

  result = C_Result_new_ok_P(Pass(self));
ret:
  Unref(path);
  return result;
}

C_Result* C_File_new_create_P(C_String* path, FileMode mode) {
  Ref(path);
  C_Result* result;

  C_Ptr* cstr = C_String_to_cstr(path);
  int descriptor =
    open(C_Ptr_get_ptr(cstr), file_flags(mode) | O_CREAT | O_TRUNC, 0644);
  Unref(cstr);

  if (descriptor < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified,
      SV("C_File_new_create_P -> failed to create file")));
    goto ret;
  }

  C_File* self = allocate(sizeof(C_File));
  self->base = ClassObject_construct(C_File_destroy, null);

  self->descriptor = descriptor;
  self->closed = false;

  result = C_Result_new_ok_P(Pass(self));
ret:
  Unref(path);
  return result;
}

C_EmptyResult* C_File_close(C_File* self) {
  if (self->closed)
    return C_EmptyResult_new_ok();
  C_EmptyResult* result;

  int close_result = close(self->descriptor);
  if (close_result < 0) {
    result = C_EmptyResult_new_err(
      E(EG_OS_IO, E_Unspecified, SV("C_File_close -> failed to close file")));
  } else {
    self->closed = true;
    result = C_EmptyResult_new_ok();
  }

  return result;
}

void C_File_destroy(void* self) {
  C_EmptyResult* close_result = C_File_close(self);
  C_EmptyResult_force(close_result);
  Unref(close_result);
}

C_Result* /* u32 */ C_File_read_chars_R(C_File* self, ascii* chars, u32 len) {
  int read_result = read(self->descriptor, chars, len);

  C_Result* result;
  if (read_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified,
      SV("C_File_read_chars_R -> failed to read from file")));
  } else {
    result = C_Result_new_ok_P(Pass(C_Handle_u32_new(read_result)));
  }

  return result;
}

C_Result* /* u32 */ C_File_write_chars_R(C_File* self, ascii* chars, u32 len) {
  int write_result = write(self->descriptor, chars, len);

  C_Result* result;
  if (write_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified,
      SV("C_File_read_chars_R -> failed to read from file")));
  } else {
    result = C_Result_new_ok_P(Pass(C_Handle_u32_new(write_result)));
  }

  return result;
}
