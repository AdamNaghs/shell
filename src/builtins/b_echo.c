#include "../../include/cmd.h"      
#include "../../include/tokenize.h"

struct cmd_return b_echo(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    if (arr.size <= 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: echo: Missing operand."));
        return ret;
    }
    String tmp = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    str_append(&ret.str, tmp);
    str_free(tmp);
    return ret;
}