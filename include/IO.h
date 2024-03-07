#ifndef SHELL_IO
#define SHELL_IO
#include "string.h"
#include "colors.h"
#include <stdio.h> /* FILE */

/* TODO
    Get this working, right now we are using getchar
    which only gets stdin.
*/
void set_input_file(FILE *fd);

FILE* get_input_file(void);

bool at_eof(void);
/* ret_char is the char that once pressed returns the string.
    provide 0 for max_size if you do not care how long the input is */
String input(char ret_char, size_t max_size);

void disable_input_buffer_display();

void enable_input_buffer_display();

#endif /* SHELL_IO */
