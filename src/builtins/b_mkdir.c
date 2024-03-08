#include "../../include/tokenize.h"
#include "../../include/cmd.h"
#include "../../include/utils.h"
#include <stdio.h>
#define MKDIR_BUF 4096
struct cmd_return b_mkdir(Token_Array* arr, String* str)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr->size <= 1)
    {
        ret.success = false;
        ret.func_return = 1;
        goto exit_function;
    }
    size_t i;
    for (i = 1; i < arr->size; i++)
    {
        String path = arr->arr[i].str;
        if (0 != MKDIR(path.cstr))
        {
            char buf[MKDIR_BUF];
            snprintf(buf, MKDIR_BUF, "\nasn: mkdir: Could not make directory '%s'.", arr->arr[i].str.cstr);
            str_append(str, STR(buf));
        };
    }
    exit_function:
    arr->arr++;
    arr->size--;
    return ret;
}