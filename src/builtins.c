#include "../include/builtins.h"
#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <direct.h>/* getcwd chdir */

#define LS_BUF 4096

int b_echo(String_Array arr)
{
    if (arr.size == 1)
    {
        printf("\n");
        return 0;
    }
    String_Array new_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    String tmp = str_arr_join(new_arr, ' ');
    printf("%s\n", tmp.cstr);
    str_free(tmp);
    return 0;
}

int b_exit(String_Array arr)
{
    free(get_internal_cmd_list());
    printf(GRN "Exitting...\n" CRESET);
    exit(1);
    return 0;
}

int b_cd(String_Array arr)
{
    if (arr.size == 1)
    {
        return 0;
    }
    if (-1 == chdir(arr.arr[1].cstr))
    {
    };
    return 0;
}

int b_ls(String_Array arr)
{
    char buf[LS_BUF];
    char *cwd;
    cwd = getcwd(buf, LS_BUF);
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
    while (NULL != ((dir = readdir(d))))
    {
        printf("%s\n", dir->d_name);
    }
    return 0;
}

int b_help(String_Array arr)
{
    printf("help - Prints this message to stdout.\n");
    printf("exit - Exits program.\n");
    printf("echo - Prints message.\n");
    printf("cd - Change directory.\n");
    printf("ls - List files in current directory.\n");
    return 0;
}

#ifndef strdup
#define strdup _strdup
#endif

void load_builtins(void)
{
    static bool ran = false;
    if (ran)
        return;
    const char *str_echo = "echo";
    add_internal_cmd(internal_cmd_new((String){.cstr = strdup(str_echo), .size = 4}, b_echo));

    const char *str_exit = "exit";
    add_internal_cmd(internal_cmd_new((String){.cstr = strdup(str_exit), .size = 4}, b_exit));

    const char *str_cd = "cd";
    add_internal_cmd(internal_cmd_new((String){.cstr = strdup(str_cd), .size = 2}, b_cd));

    const char *str_ls = "ls";
    add_internal_cmd(internal_cmd_new((String){.cstr = strdup(str_ls), .size = 2}, b_ls));

    const char *str_help = "help";
    add_internal_cmd(internal_cmd_new((String){.cstr = strdup(str_help), .size = 4}, b_help));
    ran = true;
}