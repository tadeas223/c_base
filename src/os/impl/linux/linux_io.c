#include "base/mem.h"
#include "base/strings.h"
#include "os/os_io.h"

#include <ctime>
#include <fcntl.h>

#define DEFAULT_PERMISSIONS 755
struct File {
    s32 descriptor;
};

File*
os_io_open(m_Arena *arena, String8 path, enum FileMode mode) {
    File *file = m_arena_push(arena, sizeof(*file));

    m_Temp temp;
    m_temp_begin(arena);
    
    char* cstr = str8_to_cstr(temp.arena, path);
    s32 descriptor = open(cstr, mode, DEFAULT_PERMISSIONS);
    if (descriptor == -1) {
        /* TODO: Handle specific errors from errno*/
        return nullptr; 
    } 
    
    m_temp_end(&temp);
    return file;
}

bool
os_io_close(File *file) {
    if(close(file->descriptor)) {

    }
}
