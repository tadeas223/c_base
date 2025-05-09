#include "base/arena.h"
#include "base/mem.h"
#include "base/strdef.h"
#include "base/strings.h"
#include "base/varargs.h"
#include "os/os_io.h"
#include "os/os_mem.h"

int main(int argc, char *argv[])
{
    global_memory_base_set(os_memory_base());

    Arena arena;
    arena_init(&arena);
    
    ConsoleWrite(S("number 1: ")); 
    u32 num1 = TryParse(s32, ConsoleReadLn(&arena));
    ConsoleWrite(S("operator: "));
    u8 operator = ConsoleRead(&arena);
   
    while(ConsoleRead(&arena) != '\n') {} // flush stdin

    ConsoleWrite(S("number 2: "));

    u32 num2 = TryParse(s32, ConsoleReadLn(&arena));
    
    switch(operator) {
        case '+':
            ConsoleWriteLn(S("result: "), ToStr(s32, &arena, num1 + num2)); 
            break;
        case '-':
            ConsoleWriteLn(S("result: "), ToStr(s32, &arena, num1 - num2));
            break;
        case '*':
            ConsoleWriteLn(S("result: "), ToStr(s32, &arena, num1 * num2)); 
            break;
        case '/':
            ConsoleWriteLn(S("result: "), ToStr(s32, &arena, num1 / num2)); 
            break;
        default:
            ConsoleWriteLn(S("invalid operator"));
            break; 
    }
    
    arena_destruct(&arena);

    return 0;
}
