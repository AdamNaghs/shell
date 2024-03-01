#ifndef SHELL_IO
#define SHELL_IO
#include "string.h"
#include "colors.h"
#include <stdio.h> /* FILE */
/* needs to be freed */
typedef struct
{
    String *arr;
    size_t size;
} String_Array;

void str_arr_free(String_Array arr);

/* TODO
    Get this working, right now we are using getchar
    which only gets stdin.
*/
void set_input_file(FILE* fd);

/* ret_char is the char that once pressed returns the string.
    provide 0 for max_size if you do not care how long the input is */
String input(char ret_char, size_t max_size);

/* prints string literal or pre-formatted strings */
void print(String str, FILE *ostream);

void print_ln(String str, FILE *ostream);

/* delim is the array of seperators */
String_Array str_split(String str, String delim);

String str_arr_join(String_Array arr, char seperator);

/* old string array is not freed, neither is the string*/
String_Array str_arr_add(String_Array,String);


#endif /* SHELL_IO */