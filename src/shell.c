#include "../include/shell.h"
#include "../include/IO.h"
#include "../include/signal.h"
#include "../include/cmd.h"
#include "../include/builtins.h"
#include "../include/credentials.h"
#include "../include/var.h"
#include "../include/utils.h"
#include "../include/IO.h"
#include <stdlib.h>
#include <string.h>

static bool prelude_ran = 0;
static bool shell_run = true;

void shell_prelude(void)
{
    if (prelude_ran)
        return;
    bind_signals();
    attempt_login_loop();
    load_builtins();
    load_external_commands();
    prelude_ran = true;
    shell_run = true;
}

void shell_stop(void)
{
    ran_load_builtins = 0;
    shell_run = false;
    prelude_ran = 0;
    free_all_vars();
    free_all_commands();
}

#define CWD_BUF 4096

void shell_loop_step(bool print_input)
{
    shell_prelude();
    static char buf[2] = " ";
    static char buf1[2] = "|";
    String space_delim = STR(buf);
    String pipe_delim = STR(buf1);
    char cwd_buf[CWD_BUF];
    struct cmd_return ret = {.success = false, .func_return = 0, .str = {.cstr = NULL, .size = 0}};
    char *c = GETCWD(cwd_buf, CWD_BUF);
    if (!c)
    {
        perror(RED "Could not get current working directory.\n");
        exit(1);
    }
    printf(BLU "asn" CRESET "@" CYN "%s> " CRESET, cwd_buf);
    String a = input('\n', 0);
    if (print_input)
    {
        printf("%s", a.cstr);
    }
    if (a.size == 0 || -1 != str_contains_char(a, '#'))
    {
        printf("\n");
        str_free(a);
        return;
    }
    if (0 == read_var(a))
    {
        str_free(a);
        return;
    }
    paste_vars('$', &a);
    String_Array commands = str_split(a, pipe_delim);
    // String_Array commands;
    // STR_ARRAY_STACK_ALLOC(commands, str_count(a, pipe_delim));
    // str_split_as_view(&commands, a, pipe_delim);

    size_t cmd;
    bool ran = false;
    for (cmd = 0; cmd < commands.size; cmd++)
    {
        String_Array args = str_split(commands.arr[cmd], space_delim);
        // STR_ARRAY_STACK_ALLOC(args, str_count(commands.arr[cmd], space_delim));
        // str_split_as_view(&args, commands.arr[cmd], space_delim);
        if (!args.size)
            break;
        struct internal_cmd *int_cmd = find_internal_cmd(args.arr[0]);
        if (int_cmd)
        {
            if (cmd == 0)
            {
                ret = int_cmd->func(args);
            }
            else
            {
                String tmp = str_new(commands.arr[cmd].cstr);
                str_append(&tmp, space_delim);
                str_append(&tmp, ret.str);
                String_Array piped_command_return = str_split(tmp, space_delim);
                // STR_ARRAY_STACK_ALLOC(piped_command_return, str_count(tmp, space_delim));
                // str_split_as_view(&piped_command_return, tmp, space_delim);
                str_free(ret.str);
                ret = int_cmd->func(piped_command_return);
                str_free(tmp);
                str_arr_free(piped_command_return);
            }
            ran = true;
        }
        str_arr_free(args);
    }
    if (!ran)
        printf("Could not find command '%s'.\n", commands.arr[0].cstr);
    else
        printf("%s\n", ret.str.cstr);
    str_arr_free(commands);
    if (ret.str.cstr)
        str_free(ret.str);
    str_free(a);
}

void shell_loop_test()
{
    shell_prelude();
    while (shell_run && !at_eof())
    {
        shell_loop_step(true);
    }
    printf("\n");
}

void shell_loop(void)
{
    shell_prelude();
    while (shell_run)
    {
        shell_loop_step(false);
    }
}
