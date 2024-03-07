#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/utils.h"      
#define PWD_BUF 4096

/* unused param */
struct cmd_return b_pwd(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[PWD_BUF];
    if (!GETCWD(buf, sizeof(buf)))
    {
        ret.func_return = 1;
        return ret;
    }
    str_append(&ret.str, STR(buf));
    ret.success = true;
    return ret;
}