#ifndef SHELL_H
#define SHELL_H
#include "IO.h"

void shell_loop(void);

void shell_loop_test(void);

void shell_stop(void);

void shell_reset(void);

void shell_loop_step(bool print_output, bool print_input);

void shell_loop_manual_step(String inp, bool print_input, bool print_output, bool print_error);
#endif /* SHELL_H */
