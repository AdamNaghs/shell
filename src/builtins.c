#include "../include/var.h"
#include "../include/colors.h"
#include "../include/builtins.h" /* load_builtins*/
#include "../include/cmd.h"      /* get_internal_cmd_list, size, add*/
#include "../include/utils.h"    /* is_dir */
#include "../include/shell.h"    /* shell_stop*/
#include <stdlib.h>              /* getenv, malloc, realloc, free */
#include <string.h>
#include <time.h>

int ran_load_builtins;

int *are_builtins_loaded(void)
{
    return &ran_load_builtins;
}

struct cmd_return b_mkdir(String_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr.size <= 1)
    {
        ret.success = false;
        ret.func_return = 1;
        return ret;
    }
    String path = arr.arr[1];
    MKDIR(path.cstr);
    return ret;
}

#define PWD_BUF 4096

struct cmd_return b_cd(String_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    if (arr.size == 1)
    {
        ret.func_return = 1;
        return ret;
    }
    String str = str_arr_join((String_Array){arr.arr + 1, arr.size - 1}, ' ');
    if (str.size)
        str.cstr[str.size--] = '\0'; /* remove extra ' ' at end */
    if (-1 == CHDIR(str.cstr))
    {
        printf(RED "asn: Could not open directory " BHRED "'%s'\n" CRESET, str.cstr);
    }
    str_free(str);
    ret.success = true;
    return ret;
}

struct cmd_return b_pwd(String_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[PWD_BUF];
    if (!GETCWD(buf, sizeof(buf)))
    {
        ret.func_return = 1;
        return ret;
    }
    String tmp_str = str_new(buf);
    str_append(&ret.str, tmp_str);
    str_free(tmp_str);
    ret.success = true;
    return ret;
}
/* windows dependant */
#define LS_BUF 4096
#ifdef _WIN32
#include <windows.h>
struct cmd_return b_ls(String_Array arr)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char searchPath[LS_BUF];
    String tmp_str = str_arr_join((String_Array){arr.arr + 1, arr.size - 1}, ' ');
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
    str_append(&ret.str, new_line_str);
    do
    {
        String tmp_str = str_new(findFileData.cFileName);
        str_append(&tmp_str, new_line_str);
        str_append(&ret.str, tmp_str);
        str_free(tmp_str);
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
    if (ret.str.size)
    {
        ret.str.cstr[ret.str.size - 1] = '\0';
        ret.str.size -= 1;
    }
    ret.success = true;
    return ret;
}
#else
#include <dirent.h> /* opendir, readdir, closedir, */
struct cmd_return b_ls(String_Array arr)
{
    char new_line_char[2] = "\n";
    String new_line_str = {.cstr = new_line_char, .size = 1};
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[LS_BUF];
    char *cwd;
    cwd = GETCWD(buf, LS_BUF);
    struct dirent *dir;
    DIR *d;
    if (arr.size == 1)
    {
        if (cwd)
            d = opendir(cwd);
        else
            d = opendir(".");
    }
    else
    {
        d = opendir(arr.arr[1].cstr);
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
    ret.success = true;
    /*ret.str.cstr[ret.str.size--] = '\0';*/
    return ret;
}
#endif

/* cstdlib */
struct cmd_return b_echo(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    if (arr.size == 1)
    {
        return ret;
    }
    String_Array new_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    String tmp = str_arr_join(new_arr, ' ');
    str_append(&ret.str, tmp);
    str_free(tmp);
    return ret;
}

#define RM_BUF 4096
struct cmd_return b_rm(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    String args = str_arr_join((String_Array){arr.arr + 1, arr.size - 1}, ' ');
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

struct cmd_return b_rmdir(String_Array arr)
{
    return b_rm(arr);
}
struct cmd_return b_exit(String_Array arr)
{
    char tmp_char[1] = "";
    struct cmd_return ret = {
        .success = true,
        .func_return = 0,
        .str = str_new(tmp_char),
    };
    shell_stop();
    str_append(&ret.str, STR(GRN "\nExitting asn...\n" CRESET));
    return ret;
}

struct cmd_return b_clear(String_Array arr)
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

struct cmd_return b_osys(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;

    String_Array tmp_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    String cmd = str_arr_join(tmp_arr, ' ');

    capture_system_call(&ret, cmd);
    str_free(cmd);
    return ret;
}

struct cmd_return b_help(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    char help_buf[1000] =
        BHCYN "help" CRESET "\t- Prints this message to stdout.\n" BHCYN "exit" CRESET "\t- Exits program.\n" BHCYN "echo" CRESET "\t- Prints message.\n" BHCYN "clear" CRESET "\t- Wipes terminal.\n" BHCYN "cd" CRESET "\t- Change directory.\n" BHCYN "ls" CRESET "\t- List files in current directory.\n" BHCYN "pwd" CRESET "\t- Print working directory.\n" BHCYN "rm" CRESET "\t - Removes files.\n" BHCYN "touch" CRESET "\t - Creates files.\n" BHCYN "mkdir" CRESET "\t - Creates new direction with provided path.\n" BHCYN "rmdir" CRESET "\t - Removes directories.\n" BHCYN "asn" CRESET "\t - asn shell, used to run file containing commands.\n" BHCYN "osys" CRESET "\t- Outer system/shell call.\n" BHCYN "time" CRESET "\t - Prints runtime of its arguments or time since UNIX epoch.\n" BHCYN "reset" CRESET "\t - Resets commands & variables.\n";
    String tmp_str = str_new(help_buf);
    str_append(&ret.str, tmp_str);
    str_free(tmp_str);
    return ret;
}

struct cmd_return b_touch(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    static char open_mode[2] = "a";
    size_t i = 1;
    for (; i < arr.size; i++)
    {
        FILE *f = FOPEN(arr.arr[i].cstr, open_mode);
        if (!f)
        {
            char tmp[4096];
            sprintf(tmp, RED "asn: Failed to create file '%s'.\n" CRESET, arr.arr[i].cstr);
            String tmp_str = {tmp, 4096};
            str_append(&ret.str, tmp_str);
            continue;
        }
        FCLOSE(f);
    }
    return ret;
}

struct cmd_return b_reset(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    str_append(&ret.str, STR("\nResetting asn.\n"));
    shell_reset();
    return ret;
}

struct cmd_return b_asn(String_Array arr)
{
    clock_t start = clock();
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    size_t i;
    FILE *default_file = get_input_file();
    for (i = 1; i < arr.size; i++)
    {
        FILE *fd = FOPEN(arr.arr[i].cstr, "r");
        if (!fd)
        {
            str_append(&ret.str, STR("asn: Could not open file.\n"));
            ret.func_return = 1;
            ret.success = false;
            return ret;
        }
        set_input_file(fd);
        shell_loop_test();
        FCLOSE(fd);
    }
    shell_reset();
    set_input_file(default_file);
    // printf("Test File(s) Runtime: %lums",clock()-start);
    return ret;
}
struct cmd_return b_time(String_Array arr)
{
    struct cmd_return ret = CMD_RETURN_SUCCESS;
    if (arr.size <= 1)
    {
        char buf[256];
        snprintf(buf, 256, "%lld\n", time(NULL));
        str_append(&ret.str, STR(buf));
        return ret;
    }
    clock_t start = clock();
    String_Array tmp_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    struct internal_cmd *cmd = find_internal_cmd(tmp_arr.arr[0]);
    if (!cmd)
    {
        str_append(&ret.str, STR("asn: time: Could not find command.\n"));
        ret.func_return = 1;
        ret.success = false;
        return ret;
    }
    str_free(ret.str);
    ret = cmd->func(tmp_arr);
    char buf[256];
    snprintf(buf, 256, "\n'%s' ran in: %ldms", tmp_arr.arr[0].cstr, clock() - start);
    str_append(&ret.str, STR(buf));
    return ret;
}

void load_builtins(void)
{
    if (ran_load_builtins)
        return;
    ran_load_builtins = 1;
    char str_echo[5] = "echo";
    add_internal_cmd(internal_cmd_new(str_new(str_echo), b_echo));

    char str_exit[5] = "exit";
    add_internal_cmd(internal_cmd_new(str_new(str_exit), b_exit));

    char str_cd[3] = "cd";
    add_internal_cmd(internal_cmd_new(str_new(str_cd), b_cd));

    char str_ls[3] = "ls";
    add_internal_cmd(internal_cmd_new(str_new(str_ls), b_ls));

    char str_osys[5] = "osys";
    add_internal_cmd(internal_cmd_new(str_new(str_osys), b_osys));

    char str_clear[6] = "clear";
    add_internal_cmd(internal_cmd_new(str_new(str_clear), b_clear));

    char str_help[5] = "help";
    add_internal_cmd(internal_cmd_new(str_new(str_help), b_help));

    char str_pwd[4] = "pwd";
    add_internal_cmd(internal_cmd_new(str_new(str_pwd), b_pwd));

    char str_mkdir[6] = "mkdir";
    add_internal_cmd(internal_cmd_new(str_new(str_mkdir), b_mkdir));

    char str_rm[3] = "rm";
    add_internal_cmd(internal_cmd_new(str_new(str_rm), b_rm));

    char str_touch[6] = "touch";
    add_internal_cmd(internal_cmd_new(str_new(str_touch), b_touch));

    char str_rmdir[6] = "rmdir";
    add_internal_cmd(internal_cmd_new(str_new(str_rmdir), b_rmdir));

    char str_reset[6] = "reset";
    add_internal_cmd(internal_cmd_new(str_new(str_reset), b_reset));

    char str_asn[4] = "asn";
    add_internal_cmd(internal_cmd_new(str_new(str_asn), b_asn));

    char str_time[5] = "time";
    add_internal_cmd(internal_cmd_new(str_new(str_time), b_time));
}
