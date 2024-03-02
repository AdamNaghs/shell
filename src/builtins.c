#include "../include/builtins.h"
#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <direct.h>/* getcwd chdir */

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

#define LS_BUF 4096

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

int b_clear(String_Array arr)
{
    size_t i = 0;
    for (; i < 100; i++)
        printf("\n");
    return 0;
}

int b_osys(String_Array arr)
{
    String_Array tmp_arr = {.arr = arr.arr + 1, .size = arr.size - 1};
    String tmp = str_arr_join(tmp_arr, ' ');
    int ret = system(tmp.cstr);
    str_free(tmp);
    return ret;
}

int b_pwd(String_Array arr)
{
    char buf[LS_BUF];
    char* cwd;
    cwd = getcwd(buf,LS_BUF);
    if (!cwd) return 1;
    printf("%s\n",cwd);
    return 0;
}

int b_help(String_Array arr)
{
    printf(BHCYN "help"  CRESET "\t- Prints this message to stdout.\n");
    printf(BHCYN "exit"  CRESET "\t- Exits program.\n");
    printf(BHCYN "echo"  CRESET "\t- Prints message.\n");
    printf(BHCYN "osys"  CRESET "\t- Outer system/shell call.\n");
    printf(BHCYN "clear" CRESET "\t- Wipes terminal.\n");
    printf(BHCYN "cd"    CRESET "\t- Change directory.\n");
    printf(BHCYN "ls"    CRESET "\t- List files in current directory.\n");
    printf(BHCYN "pwd"   CRESET "\t- Print working directory.\n");
    return 0;
}

void load_builtins(void)
{
    static bool ran = false;
    if (ran)
        return;
    char *str_echo = "echo";
    add_internal_cmd(internal_cmd_new(str_new(str_echo), b_echo));

    char *str_exit = "exit";
    add_internal_cmd(internal_cmd_new(str_new(str_exit), b_exit));

    char *str_cd = "cd";
    add_internal_cmd(internal_cmd_new(str_new(str_cd), b_cd));

    char *str_ls = "ls";
    add_internal_cmd(internal_cmd_new(str_new(str_ls), b_ls));

    char *str_osys = "osys";
    add_internal_cmd(internal_cmd_new(str_new(str_osys), b_osys));

    char *str_clear = "clear";
    add_internal_cmd(internal_cmd_new(str_new(str_clear), b_clear));

    char *str_help = "help";
    add_internal_cmd(internal_cmd_new(str_new(str_help), b_help));

    char *str_pwd = "pwd";
    add_internal_cmd(internal_cmd_new(str_new(str_pwd), b_pwd));
    ran = true;
}