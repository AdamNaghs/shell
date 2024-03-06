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

#define STR_ARRAY_VIEWS

void shell_prelude(void)
{
    if (prelude_ran)
        return;
    bind_signals();
    attempt_login_loop();
    load_builtins();
    load_external_commands();
    init_var_arr();
    prelude_ran = true;
    shell_run = true;
}

void shell_stop(void)
{
    if (!prelude_ran)
        return;
    int *builins_loaded = are_builtins_loaded();
    *builins_loaded = 0;
    shell_run = false;
    prelude_ran = 0;
    free_all_vars();
    free_all_commands();
}

void shell_reset(void)
{
    if (prelude_ran)
        shell_stop();
    shell_prelude();
}

#define CWD_BUF 4096
void shell_loop_step(bool print_output, bool print_input)
{
    if (print_output)
    {
        char cwd_buf[CWD_BUF];
        char *c = GETCWD(cwd_buf, CWD_BUF);
        if (!c)
        {
            perror(RED "asn: Could not get current working directory.\n" CRESET);
            exit(1);
        }
        if (get_input_file() != stdin)
            printf("\n");
        printf(BLU "asn" CRESET "@" CYN "%s> " CRESET, cwd_buf);
    }
    String inp = input('\n', 0);

    shell_loop_manual_step(&inp, print_input, print_output, true);
    str_free(inp);
}
void shell_loop_manual_step(String* inp, bool print_input, bool print_output, bool print_error)
{
    shell_prelude();
    static char buf[2] = " ";
    static char buf1[2] = "|";
    String space_delim = STR(buf);
    String pipe_delim = STR(buf1);

    str_remove_trailing_whitespace(inp);
    struct cmd_return ret = {.success = false, .func_return = 0, .str = {.cstr = NULL, .size = 0}};
    if (print_input)
    {
        printf("%s", inp->cstr);
    }
    if (inp->size == 0 || -1 != str_contains_char(*inp, '#'))
    {
        if (print_output)
            printf("\n");
        return;
    }
    if (0 == read_var(*inp))
    {
        return;
    }
    paste_vars('$', inp);
    String_Array commands;
#ifdef STR_ARRAY_VIEWS
    STR_ARRAY_STACK_ALLOC(commands, str_count(*inp, pipe_delim));
    str_split_as_view(&commands, *inp, pipe_delim);
#else
    commands = str_split(*inp, pipe_delim);
#endif
    size_t cmd;
    bool ran = false;
    for (cmd = 0; cmd < commands.size; cmd++)
    {
        String_Array args;
#ifdef STR_ARRAY_VIEWS
        STR_ARRAY_STACK_ALLOC(args, str_count(commands.arr[cmd], space_delim));
        str_split_as_view(&args, commands.arr[cmd], space_delim);
#else
        args = str_split(commands.arr[cmd], space_delim);
#endif
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
                String tmp = str_new(NULL);
                str_append(&tmp, commands.arr[cmd]);
                str_append(&tmp, space_delim);
                str_append(&tmp, ret.str);
                String_Array piped_command_return;
#ifdef STR_ARRAY_VIEWS
                STR_ARRAY_STACK_ALLOC(piped_command_return, str_count(tmp, space_delim));
                str_split_as_view(&piped_command_return, tmp, space_delim);
#else
                piped_command_return = str_split(tmp, space_delim);
#endif
                str_free(ret.str);
                ret = int_cmd->func(piped_command_return);
#ifndef STR_ARRAY_VIEWS
                str_arr_free(piped_command_return);
#endif
                str_free(tmp);
            }
            ran = true;
        }
        else
        {
            /*
                This should probably stay commented out so the user knows
                if they are using a shell or external command
            */
            // ret = facade_internal_cmd(args);
            // ran = true;
            // printf("asn: Ran file found in path.\n");
        }
#ifndef STR_ARRAY_VIEWS
        str_arr_free(args);
#endif
    }
    if (print_output)
    {
        if (!ran)
        {
            if (print_error)
            {
                if (get_input_file() != stdin)
                    printf("\n");
                printf("asn: Could not find command '%s'.\n", commands.arr[0].cstr);
            }
        }
        else
            printf("\n%s\n", ret.str.cstr);
    }
#ifndef STR_ARRAY_VIEWS
    str_arr_free(commands);
#endif
    if (ret.str.cstr)
        str_free(ret.str);
}

void shell_loop_test(void)
{
    shell_prelude();
    while (shell_run && !at_eof())
    {
        shell_loop_step(true, true);
    }
}

void shell_loop(void)
{
    shell_prelude();
    while (shell_run)
    {
        shell_loop_step(true, false);
    }
}
