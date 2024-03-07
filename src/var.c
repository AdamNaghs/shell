#include "../include/var.h"
#include "../include/string_array.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

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
/* returns null if it does not have it*/
Variable *get_value(String name)
{
    size_t i;
    String tmp = str_new(NULL);
    str_append(&tmp, name);
    for (i = 0; i < size; i++)
    {
        if (str_equal(tmp, var_arr[i].name))
        {
            str_free(tmp);
            return &var_arr[i];
        }
    }
    str_free(tmp);
    return NULL;
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
    if (-1 != str_contains_char(inp, ' '))
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
    Variable *v = get_value(arr.arr[0]);
    if (NULL != v)
    {
        str_free(v->value);
        v->value = str_new(arr.arr[1].cstr);
    }
    else
    {
        add_var(var_new(arr.arr[0], arr.arr[1]));
    }
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
    cap = 10;
    ran = 0;
}

/* will look for and replace variables that are prefixed by 'prefix'*/
void paste_vars(char prefix, String *string)
{
    signed long long idx = str_contains_char(*string, prefix);
    if (idx == -1 || (unsigned long long)idx + 1 >= string->size || !isalpha(string->cstr[idx + 1]))
    {
        /* Either no prefix found, or the next character after the prefix is not a letter*/
        return;
    }
    if (size == 0) return;

    String ret = str_new(NULL);
    str_append(&ret, (String){.cstr = string->cstr, .size = (size_t)idx});

    /* Find the end of the variable name */
    size_t end_idx = idx + 1;
    while (end_idx < string->size && (isalpha(string->cstr[end_idx]) || isdigit(string->cstr[end_idx])))
    {
        end_idx++;
    }

    /* Extract the variable name */
    String var_name = {.cstr = string->cstr + idx + 1, .size = (unsigned long long)end_idx - idx - 1};
    Variable *var = get_value(var_name);

    /* Append the value or the variable name itself if value not found */
    if (var)
    {
        str_append(&ret, var->value);
    }
    else
    {
        str_append(&ret, var_name);
    }

    /* Append the rest of the original string */
    if (end_idx < string->size)
    {
        str_append(&ret, (String){.cstr = string->cstr + end_idx, .size = string->size - end_idx});
    }

    String temp = *string;
    *string = ret;
    ret = temp;
    str_free(ret);

    /* Check if there are more variables to replace */
    paste_vars(prefix, string);
}
