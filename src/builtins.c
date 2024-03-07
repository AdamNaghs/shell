#include "../include/var.h"
#include "../include/colors.h"
#include "../include/builtins.h" /* load_builtins*/
#include "../include/cmd.h"      /* get_internal_cmd_list, size, add*/
#include "../include/utils.h"    /* is_dir */
#include "../include/shell.h"    /* shell_stop*/
#include "../include/tokenize.h"
#include <stdlib.h> /* getenv, malloc, realloc, free */
#include <string.h>
#include <time.h>

int ran_load_builtins;

int *are_builtins_loaded(void)
{
    return &ran_load_builtins;
}

#define MKDIR_BUF 4096
struct cmd_return b_mkdir(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr.size <= 1)
    {
        ret.success = false;
        ret.func_return = 1;
        return ret;
    }
    size_t i;
    for (i = 1; i < arr.size; i++)
    {
        String path = arr.arr[i].str;
        if (0 != MKDIR(path.cstr))
        {
            char buf[MKDIR_BUF];
            snprintf(buf, MKDIR_BUF, "\nasn: mkdir: Could not make directory '%s'.", arr.arr[i].str.cstr);
            str_append(&ret.str, STR(buf));
        };
    }
    return ret;
}

#define PWD_BUF 4096

struct cmd_return b_cd(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr.size == 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: cd: Missing operand."));
        return ret;
    }
    /* ignore first arg which is 'cd' */
    String str = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    if (str.size)
        str.cstr[str.size--] = '\0'; /* remove extra ' ' at end */
    str_remove_trailing_whitespace(&str);
    if (-1 == CHDIR(str.cstr))
    {
        printf( "asn: Could not open directory '%s'\n" , str.cstr);
    }
    str_free(str);
    ret.success = true;
    return ret;
}
/* unused param */
struct cmd_return b_pwd(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[PWD_BUF];
    if (!GETCWD(buf, sizeof(buf)))
    {
        ret.func_return = 1;
        return ret;
    }
    str_append(&ret.str, STR(buf));
    ret.success = true;
    return ret;
}
#define LS_BUF 1024
#ifdef _WIN32
#include <windows.h>
struct cmd_return b_ls(Token_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char searchPath[LS_BUF];
    String tmp_str = token_array_to_str(arr, ' ');
    if (tmp_str.size && tmp_str.cstr[tmp_str.size - 1] == ' ')
        tmp_str.cstr[--tmp_str.size] = '\0';
    if (arr.size == 1) /* Use the current directory if no arguments are provided */
        snprintf(searchPath, LS_BUF, ".\\*");
    else /* Use the provided directory path */
        snprintf(searchPath, LS_BUF, "%s\\*", tmp_str.cstr);
    str_free(tmp_str);
    /* Find the first file in the directory */
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("asn: Unable to open directory '%s'\n", searchPath);
        ret.func_return = 1;
        return ret;
    }
    char new_line_char[2] = "\n";
    String new_line_str = {.cstr = new_line_char, .size = 1};
    do
    {
        tmp_str = str_new(findFileData.cFileName);
        str_append(&tmp_str, new_line_str);
        str_append(&ret.str, tmp_str);
        str_free(tmp_str);
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
    str_remove_trailing_whitespace(&ret.str);
    ret.success = true;
    return ret;
}
#else
#include <dirent.h> /* opendir, readdir, closedir, */
struct cmd_return b_ls(Token_Array cmd_inp)
{

    char new_line_char[2] = "\n";
    String new_line_str = {.cstr = new_line_char, .size = 1};
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[LS_BUF];
    char *cwd;
    cwd = GETCWD(buf, LS_BUF);
    struct dirent *dir;
    DIR *d;
    if (cmd_inp.size == 1)
    {
        if (cwd)
            d = opendir(cwd);
        else
            d = opendir(".");
    }
    else
    {
        d = opendir(cmd_inp.arr[1].str.cstr);
    }
    if (!d)
        return ret;
    while (NULL != ((dir = readdir(d))))
    {
        String tmp_str = str_new(dir->d_name);
        str_append(&tmp_str, new_line_str);
        str_append(&ret.str, tmp_str);
        str_free(tmp_str);
    }
    if (d)
    {
        closedir(d);
    }
    str_remove_trailing_whitespace(&ret.str);
    ret.success = true;
    /*ret.str.cstr[ret.str.size--] = '\0';*/
    return ret;
}
#endif

/* cstdlib */
struct cmd_return b_echo(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    if (arr.size <= 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: echo: Missing operand."));
        return ret;
    }
    String tmp = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    str_append(&ret.str, tmp);
    str_free(tmp);
    return ret;
}

struct cmd_return b_rm(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (arr.size <= 1)
    {
        ret.func_return = 1;
        str_append(&ret.str, STR_LIT("asn: rm: Missing operand."));
        return ret;
    }

    String args = token_array_to_str((Token_Array){arr.arr + 1, arr.size - 1}, ' ');
    if (is_dir(args.cstr))
    {
        ret.func_return = RMDIR(args.cstr);
    }
    else
    {
        ret.func_return = remove(args.cstr);
    }
    str_free(args);
    return ret;
}

struct cmd_return b_rmdir(Token_Array arr)
{
    return b_rm(arr);
}
/* unused param */
struct cmd_return b_exit(Token_Array arr)
{
    char tmp_char[1] = "";
    struct cmd_return ret = {
        .success = true,
        .func_return = 0,
        .str = str_new(tmp_char),
    };
    shell_stop();
    str_append(&ret.str, STR(GRN "Exitting asn..." CRESET));
    return ret;
}
/* unused param */
struct cmd_return b_clear(Token_Array arr)
{
    char tmp_char[1] = "";
    struct cmd_return ret = {
        .success = true,
        .func_return = 0,
        .str = str_new(tmp_char),
    };

#ifdef _WIN32
    ret.func_return = system("cls");
#else
    ret.func_return = system("clear");
#endif
    return ret;
}

struct cmd_return b_osys(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    capture_system_call(&ret, arr);
    return ret;
}
/* unused param */
struct cmd_return b_help(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char help_buf[1000] =
        BHCYN "help" CRESET "\t- Prints this message to stdout.\n" BHCYN "exit" CRESET "\t- Exits program.\n" BHCYN "echo" CRESET "\t- Prints message.\n" BHCYN "clear" CRESET "\t- Wipes terminal.\n" BHCYN "cd" CRESET "\t- Change directory.\n" BHCYN "ls" CRESET "\t- List files in current directory.\n" BHCYN "pwd" CRESET "\t- Print working directory.\n" BHCYN "rm" CRESET "\t- Removes files.\n" BHCYN "touch" CRESET "\t- Creates files.\n" BHCYN "mkdir" CRESET "\t- Creates new direction with provided path.\n" BHCYN "rmdir" CRESET "\t- Removes directories.\n" BHCYN "asn" CRESET "\t- asn shell, used to run file containing commands.\n" BHCYN "osys" CRESET "\t- Outer system/shell call.\n" BHCYN "time" CRESET "\t- Prints runtime of its arguments or time since UNIX epoch.\n" BHCYN "reset" CRESET "\t- Resets commands & variables.";
    String tmp_str = str_new(help_buf);
    str_append(&ret.str, tmp_str);
    str_free(tmp_str);
    return ret;
}

struct cmd_return b_touch(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    static char open_mode[2] = "a";
    size_t i = 1;
    for (; i < arr.size; i++)
    {
        FILE *f = FOPEN(arr.arr[i].str.cstr, open_mode);
        if (!f)
        {
            char tmp[4096];
            sprintf(tmp, "asn: Failed to create file '%s'.", arr.arr[i].str.cstr);
            String tmp_str = {tmp, 4096};
            str_append(&ret.str, tmp_str);
            continue;
        }
        FCLOSE(f);
    }
    return ret;
}

struct cmd_return b_reset(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    str_append(&ret.str, STR_LIT("Resetting asn."));
    shell_reset();
    return ret;
}

struct cmd_return b_asn(Token_Array arr)
{
    // clock_t start = clock();
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char mode[2] = "r";
    size_t i;
    FILE *def_file = get_input_file();
    for (i = 1; i < arr.size; i++)
    {
        FILE *fd = FOPEN(arr.arr[i].str.cstr, mode);
        if (!fd)
        {
            str_append(&ret.str, STR_LIT("asn: Could not open file."));
            ret.func_return = 1;
            ret.success = false;
            set_input_file(def_file);
            return ret;
        }
        set_input_file(fd);
        shell_loop_test();
        if (0 != FCLOSE(fd))
        {
            str_append(&ret.str, STR_LIT("asn: Could not close file."));
            ret.func_return = 1;
            ret.success = false;
            set_input_file(def_file);
            return ret;
        }
    }
    set_input_file(def_file);
    // printf("Test File(s) Runtime: %lums",clock()-start);
    return ret;
}
#define TIME_BUF 64

struct cmd_return b_time(Token_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char buf[TIME_BUF];
    if (arr.size <= 1)
    {
        snprintf(buf, TIME_BUF, "%lld", time(NULL));
        str_append(&ret.str, STR(buf));
        return ret;
    }
    clock_t start = clock();
    Token_Array tmp_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    struct internal_cmd *cmd = find_internal_cmd(tmp_arr.arr[0].str);
    if (!cmd)
    {
        str_append(&ret.str, STR_LIT("asn: time: Could not find command.\n"));
        ret.func_return = 1;
        ret.success = false;
        return ret;
    }
    str_free(ret.str);
    ret = cmd->func(tmp_arr);
    double runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    if (ret.str.size)
        snprintf(buf, TIME_BUF, "\n'%s' ran in: %lfs", tmp_arr.arr[0].str.cstr, runtime);
    else
        snprintf(buf, TIME_BUF, "'%s' ran in: %lfs", tmp_arr.arr[0].str.cstr, runtime);
    str_append(&ret.str, STR(buf));
    return ret;
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
}
