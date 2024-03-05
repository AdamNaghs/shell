#include "../include/string_array.h"
#include "../include/colors.h"
#include <stdlib.h>
#include <string.h>

void str_arr_free(String_Array arr)
{
    if (!arr.arr)
        return;
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

void str_arr_replace(String_Array arr, size_t idx, String new_str)
{
    str_free(arr.arr[idx]);
    arr.arr[idx] = str_new(new_str.cstr);
}
String_Array str_split(String str, String delim)
{
    String_Array ret = {.arr = (String *)malloc(100 * sizeof(String)), .size = 100};
    size_t word_start = 0, delim_idx, i, j, found = 0;
    /* Skip leading delimiters */
    while (word_start < str.size && strchr(delim.cstr, str.cstr[word_start]) != NULL)
    {
        word_start++;
    }
    for (i = word_start; i < str.size; i++)
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
                String* tmp = (String *)realloc(ret.arr, ret.size * sizeof(String));
                if (!tmp)
                {
                    perror(RED "'str_split' could not realloc ret.arr 0.\n" CRESET);
                    exit(1);
                }
                ret.arr = tmp;
            }
            ret.arr[found++] = new_str;
        }
    }

    /* Skip trailing delimiters for the last element in the array */
    if (found > 0)
    {
        size_t last_idx = found - 1;
        while (ret.arr[last_idx].size > 0 && strchr(delim.cstr, ret.arr[last_idx].cstr[ret.arr[last_idx].size - 1]) != NULL)
        {
            ret.arr[last_idx].size--;
        }
        ret.arr[last_idx].cstr[ret.arr[last_idx].size] = '\0';
    }
    ret.size = found;
    if (!ret.size)
    {
        if (ret.arr)
            free(ret.arr);
        return ret;
    }
    ret.arr = (String *)realloc(ret.arr, sizeof(String) * ret.size);
    if (!ret.arr)
    {
        perror(RED "'str_split' could not realloc ret.arr 1.\n" CRESET);
        exit(1);
    }
    return ret;
}
/* create a str array which uses str the memroy of str 
    The returned string array doesn't need to be freed but the str does.
*/
void str_split_as_view(String_Array* arr, String str, String delim)
{
    size_t word_start = 0, delim_idx, i, j, found = 0;

    /* Skip leading delimiters */
    while (word_start < str.size && strchr(delim.cstr, str.cstr[word_start]) != NULL)
    {
        word_start++;
    }

    for (i = word_start; i < str.size; i++)
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
            if (found >= arr->size)
            {
                perror(RED "Provided String_Array is not large enough to contain input.\n" CRESET);
                exit(1);
            }

            str.cstr[delim_idx] = '\0'; // Place null terminator at the delimiter index
            arr->arr[found] = (String){.cstr = str.cstr + word_start, .size = delim_idx - word_start};

            // Remove leading delimiters for the first substring
            if (found == 0)
            {
                while (arr->arr[found].size > 0 && strchr(delim.cstr, arr->arr[found].cstr[0]) != NULL)
                {
                    arr->arr[found].cstr++;
                    arr->arr[found].size--;
                }
            }

            // Remove trailing delimiters for all substrings and add null terminator
            size_t end_idx = arr->arr[found].cstr - str.cstr + arr->arr[found].size;
            while (end_idx > 0 && strchr(delim.cstr, str.cstr[end_idx - 1]) != NULL)
            {
                end_idx--;
            }
            arr->arr[found].size = end_idx - (arr->arr[found].cstr - str.cstr);
            str.cstr[end_idx] = '\0';

            found++;
            word_start = delim_idx + 1; // Move past the delimiter
        }
    }

    /* Skip trailing delimiters for the last element in the array */
    if (found > 0)
    {
        size_t last_idx = found - 1;
        size_t last_char_idx = arr->arr[last_idx].cstr - str.cstr + arr->arr[last_idx].size;

        while (last_char_idx < str.size && strchr(delim.cstr, str.cstr[last_char_idx]) != NULL)
        {
            last_char_idx++;
        }

        arr->arr[last_idx].size = last_char_idx - (arr->arr[last_idx].cstr - str.cstr);

        /* Add null terminator after skipping trailing delimiters */
        if (arr->arr[last_idx].size > 0)
        {
            str.cstr[arr->arr[last_idx].cstr - str.cstr + arr->arr[last_idx].size] = '\0';
        }
    }
}