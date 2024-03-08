#include "../../include/cmd.h"
#include "../../include/tokenize.h"

struct cmd_return b_echo(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    if (arr->size <= 0)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: echo: Missing operand."));
        return ret;
    }
    Token *arg;
    while (arr->size && !is_operator(arr->arr[0].str) && (arg = consume_first_token(arr)) != NULL)
    {
        str_append(str, arg->str);
        str_append(str, STR_LIT(" "));
    }
    return ret;
}