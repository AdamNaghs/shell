#include "../include/cmd.h"
#include "../include/builtins.h"
#include "../include/utils.h"
#include "../include/colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



size_t cmd_arr_cap = 10;
size_t cmd_arr_len = 0;
struct internal_cmd *cmd_arr;
#define CPT_SYS_CALL_BUF 4096
void capture_system_call(struct cmd_return *ret, Token_Array arr)
{
    String new_cmd = token_array_to_str(arr, ' ');
    FILE *pipe = POPEN(new_cmd.cstr, "r");
    str_free(new_cmd);
    if (!pipe)
    {
        /* Handle popen failure */
        ret->success = false;
        ret->func_return = 1;
        return;
    }
    char buffer[CPT_SYS_CALL_BUF];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        str_append(&ret->str, STR(buffer));
    }
    ret->func_return = PCLOSE(pipe);
}

struct internal_cmd internal_cmd_new(String str, internal_cmd_func func)
{
    return (struct internal_cmd){
        .name = str,
        .func = func,
    };
}

void free_all_commands(void)
{
    struct internal_cmd *list = get_internal_cmd_list();
    size_t i;
    for (i = 0; i < get_internal_cmd_list_size(); i++)
    {
        str_free(list[i].name);
    }
    cmd_arr_len = 0;
    free(list);
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
        struct internal_cmd *tmp = (struct internal_cmd *)realloc(cmd_arr, cmd_arr_cap * sizeof(struct internal_cmd));
        if (!tmp)
        {
            perror("'add_internal_cmd' could not realloc cmd_arr.\n");
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

struct cmd_return facade_internal_cmd(Token_Array cmd)
{
    struct cmd_return ret = {.success = true, .func_return = 0, .str = str_new(NULL)};
    capture_system_call(&ret, cmd);
    return ret;
}
void load_external_from_file(char *filename)
{
    if (!filename) return;
    String cmd = str_new(filename);
#ifdef _WIN32
    char tmp[5] = ".exe";
#else
    char tmp[5] = ".app";
#endif
    signed long long idx = str_contains_str(cmd, STR(tmp));
    if (idx != -1)
    {
        cmd.size = idx;
        cmd.cstr[idx] = '\0';
    }
    add_internal_cmd(internal_cmd_new(cmd, facade_internal_cmd));
#ifdef LOAD_EXTERNALS_DEBUG
    printf("Added command '%s'\n", cmd.cstr);
#endif /* LOAD_EXTERNALS_DEBUG */
}
#ifdef _WIN32
#include <windows.h>
void load_external_from_folder(String str)
{
    char star[2] = "*";
    String star_str = {.cstr = star, .size = 1};
    char slash[2] = "\\";
    String slash_str = {.cstr = slash, .size = 1};
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    String tmp = str_new(str.cstr);
    if (str.size && str.cstr[str.size - 1] != '\\')
    {
        str_append(&tmp, slash_str);
    }
    str_append(&tmp, star_str);
    hFind = FindFirstFile(tmp.cstr, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        /* assume it is a file */
        load_external_from_file(str.cstr);
        return;
    }
    else
    {
#ifdef LOAD_EXTERNALS_DEBUG
        printf("Found dir '%s'\n", str.cstr);
#endif /* LOAD_EXTERNALS_DEBUG */
    }
    do
    {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        }
        load_external_from_file(findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);
    str_free(tmp);
}
#else /* _WIN32 */
#include <dirent.h>
#include <sys/stat.h>
/* call load_external_from_file once I get a file name in the folder */
void load_external_from_folder(String str)
{
    struct dirent *dir_entry;
    DIR *d;
    d = opendir(str.cstr);
    if (!d)
    {
        printf(RED "Could not open directory '%s'.\n" CRESET, str.cstr);
        return;
    }
    while ((dir_entry = readdir(d)) != NULL)
    {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", str.cstr, dir_entry->d_name);

        load_external_from_file(dir_entry->d_name);
#ifdef LOAD_EXTERNALS_DEBUG
        printf("Added command '%s'\n", path);
#endif
    }
    closedir(d);
}
#endif /* _WIN32 */

void load_external_commands(void)
{
    char tmp_buf[5] = "PATH";
    char *path = GETENV(tmp_buf);
#ifdef LOAD_EXTERNALS_DEBUG
    printf("Loading files in path: '%s'\n", path);
#endif
    String tmp_str = STR(path);
#ifdef _WIN32
    char delim[2] = ";";
#else
    char delim[2] = ":";
#endif
    String path_delim = STR(delim);
    String_Array folders = str_split(tmp_str, path_delim);
    size_t i;
    for (i = 0; i < folders.size; i++)
    {
        if (is_dir(folders.arr[i].cstr))
            load_external_from_folder(folders.arr[i]);
        else
            load_external_from_file(folders.arr[i].cstr);
    }
    str_arr_free(folders);
}

struct internal_cmd *find_internal_cmd(String name)
{
    size_t i;
    for (i = 0; i < cmd_arr_len; i++)
    {
        if (str_equal(name, cmd_arr[i].name))
        {
            return &cmd_arr[i];
        }
    }
    return NULL;
}
