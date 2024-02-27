#include "include/string.h"
#include <stdlib.h>

void str_free(String str)
{
    free(str.cstr);
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