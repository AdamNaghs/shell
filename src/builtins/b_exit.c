#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/string.h"
#include "../../include/shell.h"
#include "../../include/colors.h"
/* unused param */
struct cmd_return b_exit(Token_Array arr)
{
    char tmp_char[1] = "";
    struct cmd_return ret = {
        .success = true,
        .func_return = 0,
        .str = str_new(tmp_char),
    };
    shell_stop();
    str_append(&ret.str, STR(GRN "Exitting asn..." CRESET));
    return ret;
}