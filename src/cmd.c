#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "../include/builtins.h"
size_t cmd_arr_cap = 10;
size_t cmd_arr_len = 0;
struct internal_cmd *cmd_arr;

struct internal_cmd internal_cmd_new(String str, internal_cmd_func func)
{
    return (struct internal_cmd){
        .name = str,
        .func = func,
    };
}

void add_internal_cmd(struct internal_cmd cmd)
{
    if (cmd_arr_len == 0)
    {
        cmd_arr = (struct internal_cmd *)calloc(cmd_arr_cap, sizeof(struct internal_cmd));
    }
    if (cmd_arr_len >= cmd_arr_cap)
    {
        cmd_arr_cap *= 2;
        struct internal_cmd *tmp = (struct internal_cmd *)realloc(cmd_arr, cmd_arr_cap);
        if (!tmp)
        {
            perror(RED "'add_internal_cmd' could not realloc cmd_arr.\n");
            exit(1);
        }
        cmd_arr = tmp;
    }
    cmd_arr[cmd_arr_len] = cmd;
    cmd_arr_len++;
}

struct internal_cmd *get_internal_cmd_list(void)
{
    return cmd_arr;
}

size_t get_internal_cmd_list_size(void)
{
    return cmd_arr_len;
}
