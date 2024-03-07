#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/shell.h"

struct cmd_return b_reset(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    str_append(&ret.str, STR_LIT("Resetting asn."));
    shell_reset();
    return ret;
}