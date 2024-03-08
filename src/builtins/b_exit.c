#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/string.h"
#include "../../include/shell.h"
#include "../../include/colors.h"
/* unused param */
struct cmd_return b_exit(Token_Array* arr, String* str)
{
    char tmp_char[1] = "";
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    shell_stop();
    str_append(str, STR(GRN "Exitting asn..." CRESET));
    arr->arr++;
    arr->size--;
    return ret;
}