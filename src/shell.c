#include "../include/shell.h"
#include "../include/IO.h"
#include "../include/signal.h"
#include "../include/cmd.h"
#include "../include/builtins.h"
#include "../include/credentials.h"
#include "../include/var.h"
#include <stdlib.h>
#include <string.h>

void shell_loop(void)
{
    bind_signals();
    attempt_login_loop();
    load_builtins();
    load_external_commands();
    char buf[2] = " ";
    char buf1[2] = "|";
    String space_delim = STR(buf);
    String pipe_delim = STR(buf1);
    struct internal_cmd *cmd_list = get_internal_cmd_list();
    while (1)
    {
        struct cmd_return ret = {.success = false, .func_return = 0, .str = {.cstr = NULL, .size = 0}};
        printf(BLU "asn@tmpUser> " CRESET);
        String a = input('\n', 0);
        if (a.size == 0)
        {
            str_free(a);
            continue;
        }
        if (0 == read_var(a))
        {
            str_free(a);
            continue;
        }
        paste_vars('$', &a);
        //String_Array commands = str_split(a, pipe_delim);
        String_Array commands;
        STR_ARRAY_STACK_ALLOC(commands,str_count(a,pipe_delim));
        str_split_as_view(&commands,a,pipe_delim);
        

        size_t i = 0, cmd = 0;
        bool ran = false;
        for (; cmd < commands.size; cmd++)
        {
            String_Array args;// = str_split(commands.arr[cmd], space_delim);
            STR_ARRAY_STACK_ALLOC(args,str_count(a,space_delim));
            str_split_as_view(&args,commands.arr[cmd],space_delim);
            if (!args.size)
                break;
            for (; i < get_internal_cmd_list_size(); i++)
            {
                if (str_equal(args.arr[0], cmd_list[i].name))
                {
                    if (cmd == 0)
                    {
                        ret = cmd_list[i].func(args);
                    }
                    else
                    {
                        String tmp = str_new(commands.arr[cmd].cstr);
                        str_append(&tmp, space_delim);
                        str_append(&tmp, ret.str);
                        String_Array piped_command = str_split(tmp, space_delim);
                        str_free(ret.str);
                        ret = cmd_list[i].func(piped_command);
                        str_arr_free(piped_command);
                        str_free(tmp);
                    }
                    ran = true;
                    goto break_find_cmd_loop;
                }
            }
        break_find_cmd_loop:
            //str_arr_free(args);
        }
        if (!ran)
        {
            printf("Could not find command '%s'.\n", commands.arr[0].cstr);
        }
        else
        {
            printf("%s\n", ret.str.cstr);
        }
        //str_arr_free(commands);
        str_free(a);
        if (ret.str.cstr)
            str_free(ret.str);
    }
}
