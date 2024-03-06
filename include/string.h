#ifndef SHELL_STR
#define SHELL_STR
#include <stdbool.h>
#include <stddef.h>
/* needs to be freed */
typedef struct
{
    char *cstr;
    unsigned long long size;
} String;

#define STR(cstr)  ((String){cstr, cstr_len(cstr)})

/* returns -1 if no chars of c is in str and the first occurance index of any char of c if it is */
signed long long str_contains_char(String str, char c);

signed long long str_contains_str(String str, String c);

String str_new(char* str);

void str_append(String* dest, String end);

void str_free(String str);

bool str_equal(String,String);

size_t cstr_len(char* str);

/* count occurences of delim */
size_t str_count(String str, String delim);

void str_replace_all(String* str, char find, char replace);

#endif /* SHELL_STR */
