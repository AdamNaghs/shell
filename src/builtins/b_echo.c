#include "../../include/cmd.h"      
#include "../../include/tokenize.h"

struct cmd_return b_echo(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    if (arr->size <= 1)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: echo: Missing operand."));
        return ret;
    }
    str_append(str, arr->arr[1].str);
    arr->arr+=2;
    arr->size-=2;
    return ret;
}