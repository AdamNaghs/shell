#include "../include/shell.h"
#include "../include/IO.h"
#include "../include/signal.h"
#include "../include/cmd.h"
#include "../include/builtins.h"
#include "../include/credentials.h"
#include <stdlib.h>
#include <string.h>


void shell_loop(void)
{
    bind_signals();
    attempt_login_loop();
    load_builtins();
    load_external_commands();
    char buf[2] = " ";
    String space_delim = (String){.cstr = buf, .size = 1};
    char buf1[2] = "|";
    String pipe_delim = (String){.cstr = buf1, .size = 1};
    struct internal_cmd *cmd_list = get_internal_cmd_list();
    while (1)
    {
        struct cmd_return ret = {.success = false,.func_return = 0, .str = {.cstr = NULL, .size = 0}};
        printf(BLU "asn@tmpUser> " CRESET);
        String a = input('\n', 0);
        if (a.size == 0)
            continue;
        String_Array commands = str_split(a, pipe_delim);
        str_free(a);

        size_t i = 0, cmd = 0;
        bool ran = false;
        for (; cmd < commands.size; cmd++)
        {
            String_Array args = str_split(commands.arr[cmd], space_delim);
            for (; i < get_internal_cmd_list_size(); i++)
            {
                if (0 == strcmp(args.arr[0].cstr, cmd_list[i].name.cstr))
                {
                    if (cmd == 0)
                    {
                        ret = cmd_list[i].func(args);
                    }
                    else
                    {
                        String tmp = str_new(commands.arr[cmd].cstr);
                        str_append(&tmp,space_delim);
                        str_append(&tmp,ret.str);
                        String_Array piped_command = str_split(tmp,space_delim);
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
            str_arr_free(args);
        }
        if (!ran)
        {
            printf("Could not find command '%s'.\n", commands.arr[0].cstr);
        }
        else
        {
            printf("%s\n", ret.str.cstr);
        }
        str_arr_free(commands);
        if (ret.str.cstr)
            str_free(ret.str);
    }
}
