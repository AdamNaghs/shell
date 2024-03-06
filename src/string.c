#include "../include/string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void str_free(String str)
{
    free(str.cstr);
}

char *mystrdup(char *str, unsigned long long *ret_size)
{
    size_t cap = 50;
    char *ret = (char *)malloc(cap * sizeof(char) + 1);
    size_t i = 0;
    while (str[i] != '\0')
    {
        if (i >= cap)
        {
            cap *= 2;
            char *tmp = (char *)realloc(ret, cap * sizeof(char) + 1);
            if (!tmp)
            {
                perror("'mystrdup' could not realloc 'tmp'\n");
                exit(1);
            }
            ret = tmp;
        }
        ret[i] = str[i];
        i++;
    }
    *ret_size = (unsigned long long)i;
    char *tmp = (char *)realloc(ret, i * sizeof(char) + 1);
    if (!tmp)
    {
        perror("'mystrdup' could not realloc 'tmp'\n");
        exit(1);
    }
    ret = tmp;
    ret[i] = '\0';
    return ret;
}

String str_new(char *str)
{
    if (!str)
    {
        String ret = (String){.cstr = (char *)malloc(sizeof(char)), .size = 0};
        ret.cstr[0] = '\0';
        return ret;
    }
    String ret;
    ret.cstr = mystrdup(str, &ret.size);
    return ret;
}

signed long long str_contains_char(String str, char c)
{
    size_t i;
    for (i = 0; i < str.size; i++)
    {
        if (str.cstr[i] == c)
            return i;
    }
    return -1;
}

signed long long str_contains_str(String str, String c)
{
    signed long long i = 0, j = 0;
    for (; i < (signed long long)(str.size - c.size) + 1; i++)
    {
        int match = 0;
        for (j = 0; j < (signed long long)c.size; j++)
        {
            if (str.cstr[i + j] != c.cstr[j])
            {
                break;
            }
            if (j == (signed long long)c.size - 1)
            {
                match = 1;
            }
        }
        if (match)
        {
            return i;
        }
    }
    return -1;
}

void str_append1(String *dest, String end)
{
    if (!dest)
    {
        perror("'str_append' given NULL 'dest'.\n");
        exit(1);
    }
    size_t offset = dest->size;
    dest->size += end.size;
    char *tmp = (char *)realloc(dest->cstr, dest->size * sizeof(char) + 1);
    if (!tmp)
    {
        perror("Could not realloc 'dest' string in 'str_append'.\n");
        exit(1);
    }
    dest->cstr = tmp;
    if (NULL == memcpy(dest->cstr + offset, end.cstr, end.size * sizeof(char)))
    {
        perror("Failed to memcpy 'end' to 'dest'.\n");
        exit(1);
    }
    dest->cstr[dest->size] = '\0';
}

void str_append(String *dest, String end)
{
    if (!dest)
    {
        perror("'str_append' given NULL 'dest'.\n");
        exit(1);
    }
    if (end.size == 0 || end.cstr == NULL)
    {
        // Nothing to append
        return;
    }

    size_t offset = dest->size;
    dest->size += end.size;
    char *tmp = (char *)realloc(dest->cstr, dest->size + 1); // +1 for the null terminator
    if (!tmp)
    {
        perror("Could not realloc 'dest' string in 'str_append'.\n");
        exit(1);
    }
    dest->cstr = tmp;

    if (memcpy(dest->cstr + offset, end.cstr, end.size) == NULL)
    {
        perror("Failed to memcpy 'end' to 'dest'.\n");
        exit(1);
    }

    dest->cstr[dest->size] = '\0';
}

bool str_equal(String a, String b)
{
    if (!(a.cstr && b.cstr))
        return false;
    return a.size == b.size && !memcmp(a.cstr, b.cstr, a.size);
}

size_t str_count(String str, String delim)
{
    size_t word_start = 0, delim_idx = 0, i = 0, j = 0, found = 0;
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
            word_start = ++delim_idx;
            found++;
        }
    }
    return found;
}

void str_replace_all(String *str, char find, char replace)
{
    size_t i;
    for (i = 0; i < str->size; i++)
    {
        if (str->cstr[i] == find)
        {
            str->cstr[i] = replace;
        }
    }
}

size_t cstr_len(char *str)
{
    size_t i;
    for (i = 0; str[i] != '\0'; i++)
    {
    }
    return i;
}

void str_remove_trailing_whitespace(String *str)
{
    if (!str->size)
        return;
    size_t i = str->size - 1;
    for (; i >= 0; i--)
    {
        char *c = &str->cstr[i];
        if (*c == ' ' || *c == '\t' || *c == '\n' || *c == '\r')
        {
            *c = '\0';
            str->size--;
        }
        else
            break;
    }
}