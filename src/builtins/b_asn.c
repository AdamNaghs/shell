#include "../../include/cmd.h"      
#include "../../include/tokenize.h"
#include "../../include/IO.h"

struct cmd_return b_asn(Token_Array* arr, String* str)
{
    // clock_t start = clock();
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char mode[2] = "r";
    size_t i;
    FILE *def_file = get_input_file();
    for (i = 1; i < arr->size; i++)
    {
        FILE *fd = FOPEN(arr->arr[i].str.cstr, mode);
        if (!fd)
        {
            str_append(str, STR_LIT("asn: Could not open file."));
            ret.func_return = 1;
            ret.success = false;
            goto end_function;

        }
        set_input_file(fd);
        shell_loop_test();
        if (0 != FCLOSE(fd))
        {
            str_append(str, STR_LIT("asn: Could not close file."));
            ret.func_return = 1;
            ret.success = false;
            goto end_function;
        }
    }
    end_function:
    set_input_file(def_file);
    // printf("Test File(s) Runtime: %lums",clock()-start);
    arr->arr++;
    arr->size--;
    return ret;
}
