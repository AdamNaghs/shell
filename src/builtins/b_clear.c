#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include <stdlib.h>
/* unused param */
struct cmd_return b_clear(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

#ifdef _WIN32
    ret.func_return = system("cls");
#else
    ret.func_return = system("clear");
#endif
    consume_first_token(arr);
    return ret;
}