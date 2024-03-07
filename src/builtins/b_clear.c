#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include <stdlib.h>
/* unused param */
struct cmd_return b_clear(Token_Array arr)
{
    char tmp_char[1] = "";
    struct cmd_return ret = {
        .success = true,
        .func_return = 0,
        .str = str_new(tmp_char),
    };

#ifdef _WIN32
    ret.func_return = system("cls");
#else
    ret.func_return = system("clear");
#endif
    return ret;
}