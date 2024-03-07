#include "../../include/cmd.h"     
#include "../../include/tokenize.h"
struct cmd_return b_cd(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr.size == 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: cd: Missing operand."));
        return ret;
    }
    /* ignore first arg which is 'cd' */
    String str = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    if (str.size)
        str.cstr[str.size--] = '\0'; /* remove extra ' ' at end */
    str_remove_trailing_whitespace(&str);
    if (-1 == CHDIR(str.cstr))
    {
        printf( "asn: Could not open directory '%s'\n" , str.cstr);
    }
    str_free(str);
    ret.success = true;
    return ret;
}