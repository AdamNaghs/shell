#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/utils.h"
#include <stdio.h>

struct cmd_return b_touch(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    static char open_mode[2] = "a";
    while (consume_first_token(arr) != NULL && arr-> size &&  !is_operator(arr->arr[0]))
    {
        FILE *f = FOPEN(arr->arr[0].str.cstr, open_mode);
        if (!f)
        {
            char tmp[4096];
            sprintf(tmp, "asn: Failed to create file '%s'.", arr->arr[0].str.cstr);
            String tmp_str = {tmp, 4096};
            str_append(str, tmp_str);
            continue;
        }
        FCLOSE(f);
    }
    return ret;
}
