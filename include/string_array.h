#ifndef SHELL_STR_ARR
#define SHELL_STR_ARR
#include "string.h"

/* needs to be freed */
typedef struct
{
    String *arr;
    size_t size;
} String_Array;

#define STR_ARRAY_STACK_ALLOC(str_arr, str_size)                           \
    const size_t STR_ARRAY_SIZE##_str_arr = str_size;                            \
    String STR_ARRAY_STACK_ALLOC_SIZE_##str_arr[STR_ARRAY_SIZE##_str_arr]; \
    str_arr.arr = STR_ARRAY_STACK_ALLOC_SIZE_##str_arr;                    \
    str_arr.size = STR_ARRAY_SIZE##_str_arr;

void str_arr_free(String_Array arr);

/* delim is the array of seperators */
String_Array str_split(String str, String delim);

/* create a str array which uses str the memroy of str
    The returned string array doesn't need to be freed but the str does.
    Views don't play well with str_arr_join
*/
void str_split_as_view(String_Array *arr, String str, String delim);

String str_arr_join(String_Array arr, char seperator);

/* old string array is not freed, neither is the string*/
String_Array str_arr_add(String_Array, String);

void str_arr_replace(String_Array, size_t idx, String new_str);

#endif /* SHELL_STR_ARR */
