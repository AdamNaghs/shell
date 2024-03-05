#include "../include/cmd.h"
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "../include/builtins.h"
size_t cmd_arr_cap = 10;
size_t cmd_arr_len = 0;
struct internal_cmd *cmd_arr;

#ifndef popen
#define popen _popen
#endif
#ifndef pclose
#define pclose _pclose
#endif
void capture_system_call(struct cmd_return *ret, String command)
{
    FILE *pipe = popen(command.cstr, "r");
    if (!pipe)
    {
        // Handle popen failure
        ret->success = false;
        ret->func_return = 1;
    }
    else
    {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            String buffer_str = str_new(buffer);
            str_append(&ret->str, buffer_str);
            str_free(buffer_str);
        }
        ret->func_return = pclose(pipe);
    }
}

struct internal_cmd internal_cmd_new(String str, internal_cmd_func func)
{
    return (struct internal_cmd){
        .name = str,
        .func = func,
    };
}

void add_internal_cmd(struct internal_cmd cmd)
{
    if (cmd_arr_len == 0)
    {
        cmd_arr = (struct internal_cmd *)calloc(cmd_arr_cap, sizeof(struct internal_cmd));
    }
    if (cmd_arr_len >= cmd_arr_cap)
    {
        cmd_arr_cap *= 2;
        struct internal_cmd *tmp = (struct internal_cmd *)realloc(cmd_arr, cmd_arr_cap);
        if (!tmp)
        {
            perror(RED "'add_internal_cmd' could not realloc cmd_arr.\n");
            exit(1);
        }
        cmd_arr = tmp;
    }
    cmd_arr[cmd_arr_len] = cmd;
    cmd_arr_len++;
}

struct internal_cmd *get_internal_cmd_list(void)
{
    return cmd_arr;
}

size_t get_internal_cmd_list_size(void)
{
    return cmd_arr_len;
}

struct cmd_return facade_internal_cmd(String_Array cmd)
{
    String str = str_arr_join(cmd, ' ');
    struct cmd_return ret;
    capture_system_call(&ret, str);
    str_free(str);
    return ret;
}

#ifdef _WIN32
#include <windows.h>
void load_externals_from_folder(String *str)
{
    char star[2] = "*";
    String star_str = {.cstr = star, .size = 1};
    char slash[2] = "\\";
    String slash_str = {.cstr = slash, .size = 1};
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    if (str->size)
    {
        if (str->cstr[str->size - 1] != '\\')
        {
            str_append(str, slash_str);
        }
    }
    str_append(str, star_str);
    hFind = FindFirstFile(str->cstr, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf(RED "Unable to open directory '%s'\nFailed to load external commands.\n" CRESET, str->cstr);
        return;
    }
    else
    {
        printf("Found dir '%s'\n", str->cstr);
    }
    do
    {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        }
        else
        {
            add_internal_cmd(internal_cmd_new(str_new(findFileData.cFileName), facade_internal_cmd));
            printf("Added command '%s'\n", findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
}

void load_external_commands(void)
{
    char *path = getenv("PATH");
    printf("Loading files in path: '%s'\n", path);
    String tmp_str = {.cstr = path, .size = strlen(path)};
    char delim[2] = ";";
    String path_delim = {.cstr = delim, .size = 1};
    String_Array folders = str_split(tmp_str, path_delim);
    for (size_t i = 0; i < folders.size; i++)
    {
        load_externals_from_folder(&folders.arr[i]);
    }
}

#else

#endif