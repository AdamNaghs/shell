#ifndef SHELL_H
#define SHELL_H
#include "IO.h"

String_Array sys_call(String s);

void shell_loop(void);

#endif /* SHELL_H */