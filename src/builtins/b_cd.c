#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include "../../include/builtins.h"
struct cmd_return b_cd(Token_Array *arr, String *str)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    consume_first_token(arr);
    if (arr->size == 0)
    {
        ret.func_return = 1;
        str_append(str, STR_LIT("asn: cd: Missing operand."));
        return ret;
    }
    /* ignore first arg which is 'cd' */
    String dir_str = consume_first_token(arr)->str;
    str_remove_trailing_whitespace(&dir_str);
    if (-1 == CHDIR(dir_str.cstr))
    {
        char buf[256];
        snprintf(buf, 256, "asn: Could not open directory '%s'\n", dir_str.cstr);
        str_append(str, STR(buf));
    }
    ret.success = true;
    return ret;
}