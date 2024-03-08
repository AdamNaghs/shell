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
        perror("asn: Could not get current working directory.\n");
        exit(1);
    }
    if (get_input_file() != stdin)
        output("\n");
    if (colors_enabled())
        output(BLU "asn" CRESET "@" CYN "%s> " CRESET, cwd_buf);
    else
        output("asn@%s> ", cwd_buf);
}

void shell_print_greeting(void)
{
    if (colors_enabled())
        output("Welcome to " BLU "asn" CRESET " shell.\n\nInput " BLU "help" CRESET " for a list of commands.\n");
    else
        output("Welcome to asn shell.\n\nInput help for a list of commands.\n");
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
            output("%s", arr.arr[i].cstr);
            if (get_input_file() != stdin)
                output("\n");
        }
        shell_loop_manual_step(&arr.arr[i], print_input, print_output, true);
    }
    str_free(inp);
    str_arr_free(arr);
}
void shell_loop_manual_step(String *inp, bool print_input, bool print_output, bool print_error)
{
    if (!inp->cstr) // || !inp->size)
        return;
    shell_prelude();
    static char buf[2] = " ";
    static char buf1[2] = "|";
    String space_delim = STR(buf);
    String pipe_delim = STR(buf1);

    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (print_input)
    {
        output("%s", inp->cstr);
    }
    if (inp->size == 0 || -1 != str_contains_char(*inp, '#'))
    {
        if (print_output)
            output("\n");
        return;
    }
    /* variables must be initilized on their own line without a space between the name and equal sign and value */
    if (0 == read_var(*inp))
    {
        return;
    }
    /* varaibles are pasted in before parsing by commands */
    paste_vars('$', inp);
    Token_Array ta = tokenize(*inp);
    Token_Array line = ta;
    size_t cmd;
    bool ran = false;
    String ret_str = str_new(NULL);
    for (cmd = 0; cmd < line.size; cmd++)
    {
        struct internal_cmd *int_cmd = find_internal_cmd(line.arr[0].str);
        if (int_cmd)
        {
            ret = int_cmd->func(&line, &ret_str);
            ran = true;
        }
        else 
        {
            break;
        }
    }
    if (print_output)
    {
        if (!ran)
        {
            if (print_error)
            {
                if (get_input_file() != stdin)
                    output("\n");
                output("asn: Could not find command '%s'.\n", line.arr[0].str.cstr);
            }
        }
        else if (ret_str.size) /* ran and the command returned a non-empty string */
        {
            FILE *input_file = get_input_file();
            int colors = colors_enabled();
            if (colors)
                output(OUTPUT_COLOR);
            if (input_file != stdin)
                output("\n%s", ret_str.cstr);
            else
                output("%s\n", ret_str.cstr);
            if (colors)
                output(CRESET);
        }
    }
    free_token_array(ta);
    if (ret_str.cstr)
        str_free(ret_str);
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
    attempt_login_loop();
    shell_prelude();
    shell_print_greeting();
    while (shell_run)
    {
        shell_loop_step(true, false);
    }
}
