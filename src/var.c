#include "../include/var.h"
#include "../include/IO.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

unsigned long long size = 0;
unsigned long long cap = 10;
Variable *var_arr;
int ran = 0;

void init_var_arr(void)
{
    if (ran)
        return;
    var_arr = (Variable *)malloc(sizeof(Variable) * cap);
    ran++;
}
void add_var(Variable var)
{
    init_var_arr();
    if (size >= cap)
    {
        cap *= 2;
        Variable *tmp = (Variable *)realloc(var_arr, sizeof(Variable) * cap);
        if (!tmp)
        {
            perror("Failed to realloc variable array.\n");
            exit(1);
        }
        var_arr = tmp;
    }
    var_arr[size] = var;
    size++;
}

Variable var_new(String name, String value)
{
    return (Variable){str_new(name.cstr), str_new(value.cstr)};
}

/* return 0 if it read a variable and -1 if it did not, manages variables in an internal array */
int read_var(String inp)
{
    /* ignore leading spaces */
    if (!inp.size)
        return -1;
    while (inp.cstr[inp.size - 1] == ' ')
    {
        inp.size--;
    }
    if (str_contains_char(inp, ' '))
    {
        return -1;
    }
    signed long long idx = str_contains_char(inp, '=');
    if (-1 == idx)
    {
        return -1;
    }
    char buf[2] = "=";
    const String equals_delim = {.cstr = buf, .size = 1};
    String_Array arr = str_split(inp, equals_delim);
    if (arr.size < 2)
    {
        str_arr_free(arr);
        return -1;
    }
    add_var(var_new(arr.arr[0], arr.arr[1]));
    str_arr_free(arr);
    return 0;
}

void var_free(Variable var)
{
    str_free(var.name);
    str_free(var.value);
}

void free_all_vars(void)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        var_free(var_arr[i]);
    }
    free(var_arr);
    size = 0;
    cap = 0;
    ran = 0;
}
/* returns null if it does not have it*/
String *get_value(String name)
{
    size_t i;
    String tmp = str_new(NULL);
    str_append(&tmp, name);
    for (i = 0; i < size; i++)
    {
        if (0 == strcmp(tmp.cstr, var_arr[i].name.cstr))
        {
            str_free(tmp);
            return &var_arr[i].value;
        }
    }
    str_free(tmp);
    return NULL;
}

/* will look for and replace variables that are prefixed by 'prefix'*/
int paste_vars(String prefix, String *string)
{
    signed long long idx = str_contains_str(*string, prefix);
    if (idx == -1)
        return -1;
    String ret = str_new(NULL);
    str_append(&ret, (String){.cstr = string->cstr + idx, .size = (unsigned long long)idx});
    String tmp = {.cstr = string->cstr + idx, .size = string->size - idx};
    size_t i;
    for (i = 0; i < tmp.size && tmp.cstr[i] != '\0' && isalpha(tmp.cstr[i]) && isdigit(tmp.cstr[i]); i++)
    {
    }
    String name = {.cstr = string->cstr + idx + i, .size = (unsigned long long)idx + i - 1};
    String *value = get_value(name);
    if (!value)
    {
        str_append(&ret, (String){.cstr = string->cstr + idx + i, .size = (unsigned long long)idx + (string->size - i)});
    }
    else
    {
        str_append(&ret, *value);
        str_append(&ret, (String){.cstr = string->cstr + idx + i, .size = (unsigned long long)idx + (string->size - i)});
    }
    str_free(*string);
    *string = ret;
    return 0;
}
