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
#define STR_LIT(cstr)  ((String){cstr, sizeof(cstr) - 1})

/* returns -1 if no chars of c is in str and the first occurance index of any char of c if it is */
signed long long str_contains_char(String str, char c);

signed long long str_contains_str(String str, String c);

String str_new(char* str);

String str_new_n(char *str, size_t size);

void str_append(String* dest, String end);

void str_free(String str);

bool str_equal(String,String);

/* only use if the 'dest' memory is large enough to hold 'end' */
void str_memcpy(String* dest, size_t offset, String end);

size_t cstr_len(char* str);

/* count occurences of delim */
size_t str_count(String str, String delim);

void str_remove_trailing_whitespace(String* str);

void str_replace_all(String* str, char find, char replace);

void str_replace(String *str, String find, String replace);

#endif /* SHELL_STR */
