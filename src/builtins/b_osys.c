#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/cmd.h"

struct cmd_return b_osys(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    capture_system_call(&ret, arr);
    return ret;
}