#include "../../include/cmd.h"
#include "../../include/tokenize.h"
struct cmd_return b_cd(Token_Array *arr, String *str)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr->size == 1)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: cd: Missing operand."));
        arr->arr++;
        arr->size--;
        return ret;
    }
    /* ignore first arg which is 'cd' */
    String dir_str = token_array_to_str((Token_Array){arr->arr + 1, arr->size - 1}, ' ');
    str_remove_trailing_whitespace(&dir_str);
    if (-1 == CHDIR(dir_str.cstr))
    {
        char buf[256];
        snprintf(buf, 256, "asn: Could not open directory '%s'\n", dir_str.cstr);
        str_append(str, STR(buf));
    }
    ret.success = true;
    arr->arr += 2;
    arr->size -= 2;
    return ret;
}