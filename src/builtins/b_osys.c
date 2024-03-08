#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include "../../include/cmd.h"

struct cmd_return b_osys(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    arr->arr++;
    arr->size--;
    capture_system_call(&ret, *arr, str);
    arr->arr++;
    arr->size--;
    return ret;
}