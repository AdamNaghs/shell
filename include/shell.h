#ifndef SHELL_H
#define SHELL_H
#include "IO.h"

void shell_loop(void);

void shell_loop_step(bool print_input);

void shell_loop_test(void);

void shell_stop(void);
#endif /* SHELL_H */
