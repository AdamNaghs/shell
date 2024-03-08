#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/utils.h"
#include <stdio.h>

struct cmd_return b_touch(Token_Array* arr, String* str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    static char open_mode[2] = "a";
    size_t i = 1;
    for (; i < arr->size; i++)
    {
        FILE *f = FOPEN(arr->arr[i].str.cstr, open_mode);
        if (!f)
        {
            char tmp[4096];
            sprintf(tmp, "asn: Failed to create file '%s'.", arr->arr[i].str.cstr);
            String tmp_str = {tmp, 4096};
            str_append(str, tmp_str);
            continue;
        }
        FCLOSE(f);
    }
    return ret;
}
