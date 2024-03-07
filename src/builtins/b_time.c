#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include <time.h>
#include <stdio.h>
#define TIME_BUF 64
struct cmd_return b_time(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char buf[TIME_BUF];
    if (arr.size <= 1)
    {
        snprintf(buf, TIME_BUF, "%lld", time(NULL));
        str_append(&ret.str, STR(buf));
        return ret;
    }
    clock_t start = clock();
    Token_Array tmp_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    struct internal_cmd *cmd = find_internal_cmd(tmp_arr.arr[0].str);
    if (!cmd)
    {
        str_append(&ret.str, STR_LIT("asn: time: Could not find command.\n"));
        ret.func_return = 1;
        ret.success = false;
        return ret;
    }
    str_free(ret.str);
    ret = cmd->func(tmp_arr);
    double runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    if (ret.str.size)
        snprintf(buf, TIME_BUF, "\n'%s' ran in: %lfs", tmp_arr.arr[0].str.cstr, runtime);
    else
        snprintf(buf, TIME_BUF, "'%s' ran in: %lfs", tmp_arr.arr[0].str.cstr, runtime);
    str_append(&ret.str, STR(buf));
    return ret;
}
