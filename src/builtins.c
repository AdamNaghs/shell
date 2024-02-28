#include "../include/builtins.h"
#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h>
int b_echo(String_Array arr)
{
    if (arr.size <= 1)
    {
        printf("");
        return 1;
    }
    String_Array new_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    String tmp = str_arr_join(new_arr, ' ');
    printf("%s\n", tmp.cstr);
    str_arr_free(new_arr);
    str_free(tmp);
    return 0;
}

int b_exit(String_Array);

int b_help(String_Array);

int b_cd(String_Array);

void load_builtins(void)
{
    const char *str_echo = "echo";
    add_internal_cmd(internal_cmd_new((String){.cstr = _strdup(str_echo), .size = 4}, b_echo));

    const char *str_exit = "exit";
    add_internal_cmd(internal_cmd_new((String){.cstr = _strdup(str_exit), .size = 4}, b_exit));

    const char *str_cd = "cd";
    add_internal_cmd(internal_cmd_new((String){.cstr = _strdup(str_cd), .size = 2}, b_cd));

    const char *str_help = "help";
    add_internal_cmd(internal_cmd_new((String){.cstr = _strdup(str_help), .size = 4}, b_help));
}