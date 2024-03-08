#include "../include/var.h"
#include "../include/colors.h"
#include "../include/builtins.h" /* load_builtins*/
#include "../include/cmd.h"
#include "../include/tokenize.h"
#include "../include/utils.h" /* is_dir */
#include "../include/shell.h" /* shell_stop*/
#include <stdlib.h>           /* getenv, malloc, realloc, free */
#include <string.h>
#include <time.h>
#include "builtins/b_asn.c"
#include "builtins/b_cd.c"
#include "builtins/b_clear.c"
#include "builtins/b_echo.c"
#include "builtins/b_exit.c"
#include "builtins/b_ls.c"
#include "builtins/b_mkdir.c"
#include "builtins/b_osys.c"
#include "builtins/b_pwd.c"
#include "builtins/b_reset.c"
#include "builtins/b_rm.c"
#include "builtins/b_time.c"
#include "builtins/b_touch.c"
int ran_load_builtins;

int *are_builtins_loaded(void)
{
    return &ran_load_builtins;
}

/* unused param */
struct cmd_return b_help(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char help_buf[1000] =
        BHCYN "help" CRESET "\t- Prints this message to stdout.\n" BHCYN "exit" CRESET "\t- Exits program.\n" BHCYN "echo" CRESET "\t- Prints message.\n" BHCYN "clear" CRESET "\t- Wipes terminal.\n" BHCYN "cd" CRESET "\t- Change directory.\n" BHCYN "ls" CRESET "\t- List files in current directory.\n" BHCYN "pwd" CRESET "\t- Print working directory.\n" BHCYN "rm" CRESET "\t- Removes files.\n" BHCYN "touch" CRESET "\t- Creates files.\n" BHCYN "mkdir" CRESET "\t- Creates new direction with provided path.\n" BHCYN "rmdir" CRESET "\t- Removes directories.\n" BHCYN "asn" CRESET "\t- asn shell, used to run file containing commands.\n" BHCYN "osys" CRESET "\t- Outer system/shell call.\n" BHCYN "time" CRESET "\t- Prints runtime of its arguments or time since UNIX epoch.\n" BHCYN "reset" CRESET "\t- Resets commands & variables.";
    str_append(str, STR(help_buf));
    arr->arr++;
    arr->size--;
    return ret;
}

struct cmd_return b_write_str_to_file(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    if (!arr->size)
        return ret;
    String filepath = consume_first_token(arr)->str;
    FILE *f = FOPEN(filepath.cstr, "w");
    FILE *tmp = get_output_file();
    set_output_file(f);
    output("%s\n", str->cstr);
    set_output_file(tmp);
    ret.func_return = FCLOSE(f);
    str_free(*str);
    *str = str_new(NULL);
    return ret;
}

struct cmd_return b_append_str_to_file(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    if (!arr->size)
        return ret;
    String filepath = consume_first_token(arr)->str;
    FILE *f = FOPEN(filepath.cstr, "a");
    FILE *tmp = get_output_file();
    set_output_file(f);
    output("%s\n", str->cstr);
    set_output_file(tmp);
    ret.func_return = FCLOSE(f);
    str_free(*str);
    *str = str_new(NULL);
    return ret;
}

struct cmd_return b_pipe(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    if (!arr->size)
        return ret;
    struct internal_cmd *cmd = find_internal_cmd(arr->arr[0].str);
    if (!cmd)
    {
        // str_append(str,STR_LIT("asn: pipe: Could not find command.\n"));
        return ret;
    }
    String new_cmd = str_new(cmd->name.cstr);
    str_append(&new_cmd, STR_LIT(" "));
    str_append(&new_cmd, *str);
    str_append(&new_cmd, STR_LIT(" "));
    String back_end = token_array_to_str((Token_Array){arr->arr + 1, arr->size - 1}, ' ');
    str_append(&new_cmd, back_end);
    str_free(back_end);
    Token_Array tmp = tokenize(new_cmd);
    str_free(*str);
    *str = str_new(NULL);
    ret = cmd->func(&tmp, str);
    size_t i;
    if (tmp.size == 0)
    {
        arr->size = 0;
        return ret;
    }
    String match = tmp.arr[0].str;
    size_t end = arr->size;
    for (i = 0; i < end; i++)
    {
        if (str_equal(match, arr->arr[i].str))
            break;
        if (NULL == consume_first_token(arr))
            break;
    }
    return ret;
}

struct cmd_return b_semicolon(Token_Array *arr, String *str)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    arr->arr++;
    arr->size--;
    if (colors_enabled())
        output(OUTPUT_COLOR "%s\n" CRESET, str->cstr);
    else
        output("%s\n", str->cstr);
    str_free(*str);
    *str = str_new(NULL);
    return ret;
}
struct cmd_return b_loop(Token_Array *arr, String *str)
{
    /* takes a number(iterations), and a variable "$var_name"
        then runs the provided code
        and pastes in the iteration number anywhere it sees var_name
    */
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    consume_first_token(arr);
    Token *iter = consume_first_token(arr);
    Token *var_name = consume_first_token(arr);
    if (!iter || !var_name)
        return ret;
    size_t end = strtoull(iter->str.cstr, NULL, 10);
    if (!arr->size)
        return ret;
    struct internal_cmd *cmd = find_internal_cmd(arr->arr[0].str);
    if (!cmd)
        return ret;
    size_t i;
    for (i = 0; i < end; i++)
    {
        Token_Array tmp_arr = token_array_copy(*arr);
        Token_Array refrence = tmp_arr;
        char buf[256];
        snprintf(buf, 256, "%llu", i);
        size_t j;
        for (j = 0; j < tmp_arr.size; j++)
        {
            str_replace(&tmp_arr.arr[j].str, var_name->str, STR(buf));
        }
        do
        {
            ret = cmd->func(&tmp_arr, str);
            if (!tmp_arr.size)
                goto break_while;
            String new_cmd = tmp_arr.arr[0].str;
            cmd = find_internal_cmd(new_cmd);
            if (!cmd)
            {
                free_token_array(refrence);
                goto break_all;
            }
        } while (tmp_arr.size);
        break_while:
        free_token_array(refrence);
    }
    arr->size = 0;
break_all:
    return ret;
}

bool is_operator(String s)
{
    if (str_equal(s, STR_LIT(">")))
        return true;
    if (str_equal(s, STR_LIT(">>")))
        return true;
    if (str_equal(s, STR_LIT("|")))
        return true;
    if (str_equal(s, STR_LIT(";")))
        return true;
    return false;
}

void load_builtins(void)
{
    if (ran_load_builtins)
        return;
    ran_load_builtins = 1;
    add_internal_cmd(internal_cmd_new(str_new("echo"), b_echo));
    add_internal_cmd(internal_cmd_new(str_new("exit"), b_exit));
    add_internal_cmd(internal_cmd_new(str_new("cd"), b_cd));
    add_internal_cmd(internal_cmd_new(str_new("ls"), b_ls));
    add_internal_cmd(internal_cmd_new(str_new("osys"), b_osys));
    add_internal_cmd(internal_cmd_new(str_new("clear"), b_clear));
    add_internal_cmd(internal_cmd_new(str_new("help"), b_help));
    add_internal_cmd(internal_cmd_new(str_new("pwd"), b_pwd));
    add_internal_cmd(internal_cmd_new(str_new("mkdir"), b_mkdir));
    add_internal_cmd(internal_cmd_new(str_new("rm"), b_rm));
    add_internal_cmd(internal_cmd_new(str_new("touch"), b_touch));
    add_internal_cmd(internal_cmd_new(str_new("rmdir"), b_rmdir));
    add_internal_cmd(internal_cmd_new(str_new("reset"), b_reset));
    add_internal_cmd(internal_cmd_new(str_new("asn"), b_asn));
    add_internal_cmd(internal_cmd_new(str_new("time"), b_time));
    add_internal_cmd(internal_cmd_new(str_new("loop"), b_loop));
    add_internal_cmd(internal_cmd_new(str_new(">"), b_write_str_to_file));
    add_internal_cmd(internal_cmd_new(str_new(">>"), b_append_str_to_file));
    add_internal_cmd(internal_cmd_new(str_new("|"), b_pipe));
    add_internal_cmd(internal_cmd_new(str_new(";"), b_semicolon));
}
