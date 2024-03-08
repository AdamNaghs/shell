#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include "../../include/utils.h"
#include <stdio.h> /* remove */
struct cmd_return b_rm(Token_Array *arr, String *str)
{
    consume_first_token(arr);
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (arr->size <= 0)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: rm: Missing operand."));
        return ret;
    }
    Token *arg;
    while (arr->size && !is_operator(arr->arr[0].str) && (arg = consume_first_token(arr)) != NULL)
    {
        if (is_dir(arg->str.cstr))
        {
            ret.func_return = RMDIR(arg->str.cstr);
        }
        else
        {
            ret.func_return = remove(arg->str.cstr);
        }
        if (ret.func_return != 0)
        {
            char buf[256];
            snprintf(buf,256,"asn: rm: Failed to remove file/directory '%s'.",arg->str.cstr);
            str_append(str, STR(buf));
        }
    }
    return ret;
}

struct cmd_return b_rmdir(Token_Array *arr, String *str)
{
    return b_rm(arr, str);
}
