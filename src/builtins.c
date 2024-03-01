#include "../include/builtins.h"
#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static DIR *cwd = NULL;
String_Array cwd_str_arr = (String_Array){.arr = NULL, .size = 0};

void try_init_cwd(void)
{
    if (cwd)
        return;
    cwd = opendir(".");
    char tmp[2] = ".";
    String_Array tmp_arr = str_arr_add(cwd_str_arr, str_new(tmp));
    str_arr_free(cwd_str_arr);
    cwd_str_arr = tmp_arr;
    String tmp_str = str_arr_join(cwd_str_arr, '\\');
    cwd = opendir(tmp_str.cstr);
    str_free(tmp_str);
    if (!cwd)
    {
        printf(RED "Could not open current working directory.\n" CRESET);
        exit(1);
    }
}

void refresh_cwd(void)
{
    if (!cwd)
    {
        try_init_cwd();
        return;
    }
    closedir(cwd);
    String tmp_str = str_arr_join(cwd_str_arr, '\\');
    cwd = opendir(tmp_str.cstr);
    str_free(tmp_str);
}

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
    if (cwd)
        closedir(cwd);
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
    try_init_cwd();
    refresh_cwd();
    char leave_folder[3] = ".." ;
    String tmp = str_new(leave_folder);
    if (-1 != str_contains_str(arr.arr[1],tmp))
    {
        if (1 == cwd_str_arr.size)
        {
            printf(RED "Ascending from starting directory not yet implemented.\n" CRESET);
            return -1;
            exit(1);
        }
        String* tmp_arr_ptr = (String*) realloc(cwd_str_arr.arr,sizeof(String)*(cwd_str_arr.size - 1));
        if (!tmp_arr_ptr)
        {
            printf(RED "Could not realloc 'cwd_str_arr'.\n" CRESET);
            exit(1);
        }
        cwd_str_arr.arr = tmp_arr_ptr;
        cwd_str_arr.size--;
        return b_cd(cwd_str_arr);
    }
    str_free(tmp);
    struct dirent *dir;
    while (NULL != ((dir = readdir(cwd))))
    {
        if (0 == strcmp(arr.arr[1].cstr, dir->d_name))
        {

            String_Array tmp = str_arr_add(cwd_str_arr,str_new(arr.arr[1].cstr));
            str_arr_free(cwd_str_arr);
            cwd_str_arr = tmp;
            String tmp_str = str_arr_join(cwd_str_arr, '\\');
            if (cwd)
                closedir(cwd);
            cwd = opendir(tmp_str.cstr);
            if (!cwd)
            {
                printf(RED "Could not open directory '%s'.\n" CRESET, tmp_str.cstr);
            }
            str_free(tmp_str);
            
        }
    }
    return 0;
}

int b_ls(String_Array arr)
{
    try_init_cwd();
    refresh_cwd();
    struct dirent *dir;
    while (NULL != ((dir = readdir(cwd))))
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