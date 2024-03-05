#include "../include/string.h"
#include "../include/IO.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR 10000

static FILE *input_file = NULL;

void set_input_file(FILE *fd)
{
    if (!fd)
    {
        perror(RED "'fd' is null.\n" CRESET);
        exit(1);
    }
    input_file = fd;
}

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
    if (!input_file)
    {
        input_file = stdin;
    }
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
    while ((c = fgetc(input_file)) != enter_char && ret.size < max_size)
    {
        if (c == EOF || c == '\0')
            break;
        ret.cstr[ret.size++] = c;
    }

    ret.cstr = (char *)realloc(ret.cstr, ret.size + 1);
    if (!ret.cstr)
    {
        perror(RED "'input' could not realloc ret.cstr.\n" CRESET);
        exit(1);
    }
    ret.cstr[ret.size] = '\0';
    return ret;
}


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
void str_split_as_view(String_Array* _arr_, String _str_, String _delim_)
{
    size_t word_start = 0, delim_idx, i, j, found = 0;

    /* Skip leading delimiters */
    while (word_start < _str_.size && strchr(_delim_.cstr, _str_.cstr[word_start]) != NULL)
    {
        word_start++;
    }

    for (i = word_start; i < _str_.size; i++)
    {
        bool is_delim = false;
        for (j = 0; j < _delim_.size; j++)
        {
            if (_str_.cstr[i] == _delim_.cstr[j] || i == _str_.size - 1)
            {
                is_delim = true;
                delim_idx = i;
                if (i == _str_.size - 1)
                    delim_idx++;
                break;
            }
        }

        if (is_delim && word_start != delim_idx)
        {
            if (found >= _arr_->size)
            {
                perror(RED "Provided String_Array is not large enough to contain input.\n" CRESET);
                exit(1);
            }

            _str_.cstr[delim_idx] = '\0'; // Place null terminator at the delimiter index
            _arr_->arr[found] = (String){.cstr = _str_.cstr + word_start, .size = delim_idx - word_start};

            // Remove leading delimiters for the first substring
            if (found == 0)
            {
                while (_arr_->arr[found].size > 0 && strchr(_delim_.cstr, _arr_->arr[found].cstr[0]) != NULL)
                {
                    _arr_->arr[found].cstr++;
                    _arr_->arr[found].size--;
                }
            }

            // Remove trailing delimiters for all substrings and add null terminator
            size_t end_idx = _arr_->arr[found].cstr - _str_.cstr + _arr_->arr[found].size;
            while (end_idx > 0 && strchr(_delim_.cstr, _str_.cstr[end_idx - 1]) != NULL)
            {
                end_idx--;
            }
            _arr_->arr[found].size = end_idx - (_arr_->arr[found].cstr - _str_.cstr);
            _str_.cstr[end_idx] = '\0';

            found++;
            word_start = delim_idx + 1; // Move past the delimiter
        }
    }

    /* Skip trailing delimiters for the last element in the array */
    if (found > 0)
    {
        size_t last_idx = found - 1;
        size_t last_char_idx = _arr_->arr[last_idx].cstr - _str_.cstr + _arr_->arr[last_idx].size;

        while (last_char_idx < _str_.size && strchr(_delim_.cstr, _str_.cstr[last_char_idx]) != NULL)
        {
            last_char_idx++;
        }

        _arr_->arr[last_idx].size = last_char_idx - (_arr_->arr[last_idx].cstr - _str_.cstr);

        /* Add null terminator after skipping trailing delimiters */
        if (_arr_->arr[last_idx].size > 0)
        {
            _str_.cstr[_arr_->arr[last_idx].cstr - _str_.cstr + _arr_->arr[last_idx].size] = '\0';
        }
    }
}