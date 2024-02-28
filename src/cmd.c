#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "../include/builtins.h"
size_t cap = 10;
struct internal_cmd* cmd_arr;

struct internal_cmd internal_cmd_new(String str,internal_cmd_func func)
{
    return (struct internal_cmd){.name = str,.func = func,};
}

void add_internal_cmd(struct internal_cmd cmd)
{
    static size_t len = 0;
    if (len == 0)
    {
        cmd_arr = (struct internal_cmd*)calloc(cap,sizeof(struct internal_cmd));
    }
    if (len >= cap)
    {
        cap*=2;
        struct internal_cmd* tmp = (struct internal_cmd*)realloc(cmd_arr,cap);
        if (!tmp)
        {
            perror(RED "'add_internal_cmd' could not realloc cmd_arr.\n");
            exit(1);
        }
        cmd_arr = tmp;
    }
    cmd_arr[len] = cmd;
    len++;
}

struct cmd_return run_internal_cmd(String_Array arr)
{
    size_t i;
    struct cmd_return ret = {.success = false, .func_return = 0};
    for (i = 0; i < arr.size; i++)
    {
        if (0 == strcmp(arr.arr[0].cstr,cmd_arr->name.cstr) )
        {

            ret.func_return = cmd_arr->func(arr);
            ret.success = true;
            break;
        }
    }
    return ret;
}

void load_internal_cmd_builtins(void)
{

}

struct internal_cmd* get_internal_cmd_list(void)
{
    return cmd_arr;
}