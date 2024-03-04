#ifndef SHELL_CMD_H
#define SHELL_CMD_H
#include "IO.h"
#include <stdbool.h>

typedef struct cmd_return (*internal_cmd_func)(String_Array);

struct internal_cmd
{
    String name;
    internal_cmd_func func;
};

struct cmd_return
{
    bool success;
    int func_return;
    String str;
};

struct internal_cmd internal_cmd_new(String,internal_cmd_func);

void add_internal_cmd(struct internal_cmd);

struct internal_cmd* get_internal_cmd_list(void);

size_t get_internal_cmd_list_size(void);

#endif /* SHELL_CMD_H */