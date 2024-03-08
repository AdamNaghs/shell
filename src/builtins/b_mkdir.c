#include "../../include/tokenize.h"
#include "../../include/cmd.h"
#include "../../include/utils.h"
#include <stdio.h>
#define MKDIR_BUF 4096
struct cmd_return b_mkdir(Token_Array* arr, String* str)
{
    consume_first_token(arr);
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr->size <= 0)
    {
        ret.success = false;
        ret.func_return = 1;
        return ret;
    }
    Token* arg;
    while (arr->size && !is_operator(arr->arr[0]) && (arg = consume_first_token(arr)) != NULL)
    {
        String path = arg->str;
        if (0 != MKDIR(path.cstr))
        {
            char buf[MKDIR_BUF];
            snprintf(buf, MKDIR_BUF, "\nasn: mkdir: Could not make directory '%s'.", path.cstr);
            str_append(str, STR(buf));
        };
    }
    return ret;
}