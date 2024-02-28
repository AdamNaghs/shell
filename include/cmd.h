#ifndef SHELL_CMD_H
#define SHELL_CMD_H
#include "IO.h"
#include <stdbool.h>

typedef int (*internal_cmd_func)(String_Array);

struct internal_cmd
{
    String name;
    internal_cmd_func func;
};

struct cmd_return
{
    bool success;
    int func_return;
};

void add_internal_cmd(struct internal_cmd);

struct cmd_return run_internal_cmd(String_Array);

#endif /* SHELL_CMD_H */