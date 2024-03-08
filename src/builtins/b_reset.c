#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/shell.h"

struct cmd_return b_reset(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    str_append(str, STR_LIT("Resetting asn."));
    shell_reset();
    arr->arr++;
    arr->size--;
    return ret;
}