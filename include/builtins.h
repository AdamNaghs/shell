#ifndef SHELL_BUILTINS
#define SHELL_BUILTINS
#include <stdbool.h>

int* are_builtins_loaded(void);

void load_builtins(void);

bool is_operator(String);


#endif /* SHELL_BUILTINS */
