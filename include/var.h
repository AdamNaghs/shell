#ifndef SHELL_VAR
#define SHELL_VAR
#include "string.h"

typedef struct 
{
    String name;
    String value;
} Variable;

/* return 0 if it read a variable and -1 if it did not, manages variables in an internal array */
int read_var(String inp);

/* will look for and replace variables that are prefixed by 'prefix'*/
void paste_vars(char prefix, String* string);

void free_all_vars(void);

#endif /* SHELL_VAR */
