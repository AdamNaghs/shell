#include "../include/string.h"
#include <stdlib.h>
#include <string.h>
void str_free(String str)
{
    free(str.cstr);
}
#ifndef strdup
#define strdup _strdup
#endif

String str_new(char *str)
{
    if (!str)
    {
        String ret = (String){.cstr = (char*)malloc(sizeof(char)), .size = 0};
        ret.cstr[0] = '\0';
        return ret;
    }
    return (String){.cstr = strdup(str), .size = strlen(str)};
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

void str_append(String* dest, String end)
{
    dest->size += end.size;
    dest->cstr = (char*)realloc(dest->cstr,dest->size * sizeof(char));
    memcpy(dest->cstr + dest->size - end.size,end.cstr,end.size);
    dest->cstr[dest->size] = '\0';
}