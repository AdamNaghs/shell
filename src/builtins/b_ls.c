#include "../../include/cmd.h"
#include "../../include/tokenize.h"
#include <stdio.h>

#define LS_BUF 1024
#ifdef _WIN32
#include <windows.h>
struct cmd_return b_ls(Token_Array *arr, String *str)
{
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char searchPath[LS_BUF];
    String tmp_str;
    if (!is_operator(arr->arr[1].str))
        tmp_str = token_array_to_str((Token_Array){arr->arr + 1, arr->size - 1}, ' ');
    else
        tmp_str = str_new(NULL);
    arr->arr++;
    arr->size--;
    if (tmp_str.size && tmp_str.cstr[tmp_str.size - 1] == ' ')
        tmp_str.cstr[--tmp_str.size] = '\0';
    if (arr->size == 0 || is_operator(arr->arr[0].str)) /* Use the current directory if no arguments are provided */
        snprintf(searchPath, LS_BUF, ".\\*");
    else /* Use the provided directory path */
    {
        snprintf(searchPath, LS_BUF, "%s\\*", tmp_str.cstr);
        arr->arr++;
        arr->size--;
    }
    str_free(tmp_str);
    /* Find the first file in the directory */
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("asn: Unable to open directory '%s'\n", searchPath);
        ret.func_return = 1;
        return;
    }
    char new_line_char[2] = "\n";
    String new_line_str = {.cstr = new_line_char, .size = 1};
    do
    {
        tmp_str = str_new(findFileData.cFileName);
        str_append(&tmp_str, new_line_str);
        str_append(str, tmp_str);
        str_free(tmp_str);
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
    str_remove_trailing_whitespace(str);
    ret.success = true;
    return ret;
}
#else
#include <dirent.h> /* opendir, readdir, closedir, */
struct cmd_return b_ls(Token_Array *cmd_inp, String *str)
{

    char new_line_char[2] = "\n";
    String new_line_str = {.cstr = new_line_char, .size = 1};
    struct cmd_return ret = DEFAULT_CMD_RETURN;
    char buf[LS_BUF];
    char *cwd;
    cwd = GETCWD(buf, LS_BUF);
    struct dirent *dir;
    DIR *d;
    if (cmd_inp->size == 1)
    {
        if (cwd)
            d = opendir(cwd);
        else
            d = opendir(".");
    }
    else
    {
        d = opendir(cmd_inp->arr[1].str.cstr);
    }
    if (!d)
        return ret;
    while (NULL != ((dir = readdir(d))))
    {
        String tmp_str = str_new(dir->d_name);
        str_append(&tmp_str, new_line_str);
        str_append(str, tmp_str);
        str_free(tmp_str);
    }
    if (d)
    {
        closedir(d);
    }
    str_remove_trailing_whitespace(str);
    ret.success = true;
    /*ret.str.cstr[ret.str.size--] = '\0';*/
    cmd_inp->arr++;
    cmd_inp->size--;
    return ret;
}
#endif