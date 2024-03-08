#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include "../../include/cmd.h"

struct cmd_return b_osys(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    capture_system_call(&ret, *arr, str);
    consume_first_token(arr);
    return ret;
}