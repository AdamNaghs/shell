#ifndef SHELL_BUILTINS
#define SHELL_BUILTINS
#include <stdbool.h>
#include "tokenize.h"

int* are_builtins_loaded(void);

void load_builtins(void);

bool is_operator(Token t);


#endif /* SHELL_BUILTINS */
