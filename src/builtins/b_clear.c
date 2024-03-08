#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include <stdlib.h>
/* unused param */
struct cmd_return b_clear(Token_Array* arr, String* str)
{
    char tmp_char[1] = "";
    struct cmd_return ret = CMD_RETURN_SUCCESS;

#ifdef _WIN32
    ret.func_return = system("cls");
#else
    ret.func_return = system("clear");
#endif
    arr->arr++;
    arr->size--;
    return ret;
}