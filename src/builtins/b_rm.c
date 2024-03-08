#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/utils.h"
#include <stdio.h> /* remove */
struct cmd_return b_rm(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (arr->size <= 1)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: rm: Missing operand."));
        goto exit_function;
    }

    String args = token_array_to_str((Token_Array){arr->arr + 1, arr->size - 1}, ' ');
    if (is_dir(args.cstr))
    {
        ret.func_return = RMDIR(args.cstr);
    }
    else
    {
        ret.func_return = remove(args.cstr);
    }
    str_free(args);
    exit_function:
    arr->arr++;
    arr->size--;
    return ret;
}

struct cmd_return b_rmdir(Token_Array* arr, String* str)
{
    return b_rm(arr,str);
}
