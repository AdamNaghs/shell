#ifndef SHELL_STR
#define SHELL_STR
/* needs to be freed */
typedef struct
{
    char *cstr;
    unsigned long long size;
} String;

/* returns -1 if no chars of c is in str and the first occurance index of any char of c if it is */
signed long long str_contains_char(String str, char c);

signed long long str_contains_str(String str, String c);

String str_new(char* str);

void str_append(String* dest, String end);

void str_free(String str);

#endif /* SHELL_STR */
