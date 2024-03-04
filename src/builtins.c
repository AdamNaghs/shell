#include "../include/builtins.h" /* load_builtins*/
#include "../include/cmd.h"      /* get_internal_cmd_list, size, add*/
#include <stdlib.h>
#include <string.h>
#include <direct.h> /* getcwd, chdir */
#include <dirent.h> /* opendir, closedir*/
#include <windows.h>
/* windows dependant */
int b_cd(String_Array arr)
{
    if (arr.size == 1)
    {
        return 0;
    }
    if (-1 == chdir(arr.arr[1].cstr))
    {
        printf(RED "Could not open directory " BHRED "'%s'\n" CRESET, arr.arr[1].cstr);
    };
    return 0;
}

#define PWD_BUF 4096

int b_pwd(String_Array arr)
{
    char buf[PWD_BUF];
    char *cwd;
    cwd = getcwd(buf, PWD_BUF);
    if (!cwd)
        return 1;
    printf("%s\n", cwd);
    return 0;
}

#define LS_BUF 4096

int b_ls(String_Array arr)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char searchPath[LS_BUF];
    if (arr.size == 1)
    {
        // Use the current directory if no arguments are provided
        snprintf(searchPath, LS_BUF, ".\\*");
    }
    else
    {
        // Use the provided directory path
        snprintf(searchPath, LS_BUF, "%s\\*", arr.arr[1].cstr);
    }

    // Find the first file in the directory
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open directory '%s'\n", searchPath);
        return 1;
    }
    else
    {
        do
        {
            // Skip "." and ".." entries
            if (strcmp(findFileData.cFileName, ".") != 0 &&
                strcmp(findFileData.cFileName, "..") != 0)
            {
                printf("%s\n", findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return 0;
}

/* cstdlib */
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

int b_help(String_Array arr)
{
    printf(BHCYN "help" CRESET "\t- Prints this message to stdout.\n");
    printf(BHCYN "exit" CRESET "\t- Exits program.\n");
    printf(BHCYN "echo" CRESET "\t- Prints message.\n");
    printf(BHCYN "osys" CRESET "\t- Outer system/shell call.\n");
    printf(BHCYN "clear" CRESET "\t- Wipes terminal.\n");
    printf(BHCYN "cd" CRESET "\t- Change directory.\n");
    printf(BHCYN "ls" CRESET "\t- List files in current directory.\n");
    printf(BHCYN "pwd" CRESET "\t- Print working directory.\n");
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


