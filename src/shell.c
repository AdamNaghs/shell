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

#define OUTPUT_COLOR BHWHT
#define CWD_BUF 4096

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

void shell_print_line_flair(void)
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

void shell_print_greeting(void)
{
    printf("Welcome to "BLU "asn" CRESET " shell.\n\nInput "BLU "help"CRESET " for a list of commands.\n");
}

void shell_loop_step(bool print_output, bool print_input)
{
    if (print_output)
        shell_print_line_flair();
    String inp = input('\n', 0);
    String_Array arr = str_split(inp, STR_LIT(";"));
    size_t i;
    for (i = 0; i < arr.size; i++)
    {
        if (print_output && i)
        {
            shell_print_line_flair();
            printf("%s", arr.arr[i].cstr);
            if (get_input_file() != stdin)
                printf("\n");
        }
        shell_loop_manual_step(&arr.arr[i], print_input, print_output, true);
    }
    str_free(inp);
    str_arr_free(arr);
}
void shell_loop_manual_step(String *inp, bool print_input, bool print_output, bool print_error)
{
    shell_prelude();
    static char buf[2] = " ";
    static char buf1[2] = "|";
    String space_delim = STR(buf);
    String pipe_delim = STR(buf1);

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
    /* variables must be initilized on their own line without a space between the name and equal sign and value */
    if (0 == read_var(*inp))
    {
        return;
    }
    /* varaibles are pasted in before parsing by commands */
    paste_vars('$', inp);
    String_Array commands;

    commands = str_split(*inp, pipe_delim);
    size_t cmd;
    bool ran = false;
    for (cmd = 0; cmd < commands.size; cmd++)
    {
        Token_Array args;
        args = tokenize(commands.arr[cmd]);
        if (!args.size)
            break;
        struct internal_cmd *int_cmd = find_internal_cmd(args.arr[0].str);
        if (int_cmd)
        {
            if (cmd == 0)
            {
                ret = int_cmd->func(args);
            }
            else /* give next command the return of the last command */
            {
                size_t tmp_len = commands.arr[cmd].size + space_delim.size + ret.str.size;
                String tmp = {.cstr = (char*) malloc(tmp_len * sizeof(char) + 1),.size = tmp_len};
                str_memcpy(&tmp,0, commands.arr[cmd]);
                str_memcpy(&tmp, commands.arr[cmd].size , space_delim);
                str_memcpy(&tmp, tmp_len - ret.str.size , ret.str);
                tmp.cstr[tmp.size++] = '\0';
                Token_Array piped_command_return;
                piped_command_return = tokenize(tmp);
                str_free(ret.str);
                ret = int_cmd->func(piped_command_return);
                free_token_array(piped_command_return);
            }
            ran = true;
        }
        free_token_array(args);
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
        else if (ret.str.size) /* ran and the command returned a non-empty string */
        {
            FILE *input_file = get_input_file();
            if (input_file != stdin)
                printf(OUTPUT_COLOR "\n%s" CRESET, ret.str.cstr);
            else
                printf(OUTPUT_COLOR "%s\n" CRESET, ret.str.cstr);
        }
    }
    str_arr_free(commands);
    if (ret.str.cstr)
        str_free(ret.str);
}
/* meant to be used if you change the input file */
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
    shell_print_greeting();
    while (shell_run)
    {
        shell_loop_step(true, false);
    }
}
