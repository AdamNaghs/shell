#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/utils.h"
#include <stdio.h> /* remove */
struct cmd_return b_rm(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (arr.size <= 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: rm: Missing operand."));
        return ret;
    }

    String args = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    if (is_dir(args.cstr))
    {
        ret.func_return = RMDIR(args.cstr);
    }
    else
    {
        ret.func_return = remove(args.cstr);
    }
    str_free(args);
    return ret;
}

struct cmd_return b_rmdir(Token_Array arr)
{
    return b_rm(arr);
}
