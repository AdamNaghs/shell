#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include <time.h>
#include <stdio.h>
#define TIME_BUF 64
struct cmd_return b_time(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char buf[TIME_BUF];
    if (arr->size <= 1 || is_operator(arr->arr[1].str))
    {
        snprintf(buf, TIME_BUF, "%lld", time(NULL));
        str_append(str, STR(buf));
        arr->arr++;
        arr->size--;
        return ret;
    }
    clock_t start = clock();
    String cmd_name = arr->arr[1].str;
    Token_Array tmp_arr = {.arr = arr->arr + 1, .size = arr->size - 1};
    struct internal_cmd *cmd = find_internal_cmd(tmp_arr.arr[0].str);
    if (!cmd)
    {
        str_append(str, STR_LIT("asn: time: Could not find command.\n"));
        ret.func_return = 1;
        ret.success = false;
        goto exit_function;
    }
    str_free(*str);
    *str = str_new(NULL);
    ret = cmd->func(&tmp_arr, str);
    double runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    if (str->size)
        snprintf(buf, TIME_BUF, "\n'%s' ran in: %lfs",cmd_name.cstr, runtime);
    else
        snprintf(buf, TIME_BUF, "'%s' ran in: %lfs",cmd_name.cstr, runtime);
    str_append(str, STR(buf));
exit_function:
    *arr = tmp_arr;
    return ret;
}
