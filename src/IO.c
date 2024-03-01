#include "../include/string.h"
#include "../include/IO.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR 10000

void print(String str, FILE *ostream)
{
    if (!ostream)
    {
        perror(RED "'print' given NULL ostream.\n" CRESET);
        exit(1);
    }
    size_t i;
    for (i = 0; i < str.size; i++)
        putc(str.cstr[i], ostream);
    fflush(ostream);
}

void println(String str, FILE *ostream)
{
    if (!ostream)
    {
        perror(RED "'print' given NULL ostream.\n" CRESET);
        exit(1);
    }
    size_t i;
    for (i = 0; i < str.size; i++)
        putc(str.cstr[i], ostream);
    putc('\n', ostream);
    fflush(ostream);
}

String input(char enter_char, size_t max_size)
{
    String ret = {.cstr = NULL, .size = 0};
    if (max_size == 0)
    {
        ret.cstr = (char *)malloc(MAX_STR * sizeof(char));
        max_size = MAX_STR - 1;
    }
    else
    {
        ret.cstr = (char *)malloc(max_size * sizeof(char) + 1);
    }
    if (!ret.cstr)
    {
        perror(RED "'input' could not malloc ret.cstr.\n" CRESET);
        exit(1);
    }
    char c;
    while ((c = getchar()) != enter_char && ret.size < max_size)
    {
        ret.cstr[ret.size++] = c;
    }
    ret.cstr[ret.size] = '\0';

    ret.cstr = (char *)realloc(ret.cstr, ret.size + 1);
    if (!ret.cstr)
    {
        perror(RED "'input' could not realloc ret.cstr.\n" CRESET);
        exit(1);
    }
    return ret;
}

String_Array str_split(String str, String delim)
{
    String_Array ret = {.arr = (String *)malloc(100 * sizeof(String)), .size = 100};
    size_t word_start = 0, delim_idx, i, j, found = 0;
    for (i = 0; i < str.size; i++)
    {
        bool is_delim = false;
        for (j = 0; j < delim.size; j++)
        {
            if (str.cstr[i] == delim.cstr[j] || i == str.size - 1)
            {
                is_delim = true;
                delim_idx = i;
                if (i == str.size - 1)
                    delim_idx++;
                break;
            }
        }

        if (is_delim && word_start != delim_idx)
        {
            String new_str = {.cstr = (char *)malloc((delim_idx - word_start) * sizeof(char) + 1), .size = delim_idx - word_start};
            memcpy(new_str.cstr, str.cstr + word_start, new_str.size);
            new_str.cstr[new_str.size] = '\0';
            word_start = ++delim_idx;
            if (found >= ret.size)
            {
                ret.size *= 2;
                ret.arr = (String *)realloc(ret.arr, ret.size);
                if (!ret.arr)
                {
                    perror(RED "'split_arr' could not realloc ret.arr.\n" CRESET);
                    exit(1);
                }
            }
            ret.arr[found++] = new_str;
        }
    }
    ret.size = found;
    ret.arr = (String *)realloc(ret.arr, sizeof(String) * ret.size);
    if (!ret.arr)
    {
        perror(RED "'split_arr' could not realloc ret.arr.\n" CRESET);
        exit(1);
    }
    return ret;
}

void str_arr_free(String_Array arr)
{
    if (!arr.arr) return;
    size_t i;
    for (i = 0; i < arr.size; i++)
    {
        if (arr.arr[i].cstr)
            str_free(arr.arr[i]);
    }
    free(arr.arr);
}

String str_arr_join(String_Array arr, char seperator)
{
    /* determine length */
    size_t i, tmp = 0, len = 0;
    for (i = 0; i < arr.size; i++)
    {
        len += arr.arr[i].size + 1; /* +1 for seperator and at the end it will account for the \0 */
    }
    String ret = {.cstr = (char *)malloc(len * sizeof(char) + 1), .size = len - 1};
    for (i = 0; i < arr.size; i++)
    {
        memcpy(ret.cstr + tmp, arr.arr[i].cstr, arr.arr[i].size);
        tmp += arr.arr[i].size;
        ret.cstr[tmp] = seperator;
        tmp++;
    }
    ret.cstr[tmp] = '\0';
    return ret;
}

String_Array str_arr_add(String_Array arr, String str)
{
    String_Array ret = {.arr = (String *)malloc(sizeof(String) * arr.size + 1), .size = arr.size + 1};
    size_t i = 0;
    for (; i < arr.size; i++)
    {
        ret.arr[i] = str_new(arr.arr[i].cstr);
    }
    ret.arr[arr.size] = str_new(str.cstr);
    return ret;
}