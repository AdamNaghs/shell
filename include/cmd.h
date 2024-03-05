#ifndef SHELL_CMD_H
#define SHELL_CMD_H
#include "IO.h"
#include <stdbool.h>

/* #define LOAD_EXTERNALS_DEBUG */

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

#define DEFAULT_CMD_RETURN ((struct cmd_return){false, 1, str_new(NULL)})
#define CMD_RETURN_SUCCESS ((struct cmd_return){true, 0, str_new(NULL)})

struct internal_cmd internal_cmd_new(String, internal_cmd_func);

void add_internal_cmd(struct internal_cmd);

struct internal_cmd *get_internal_cmd_list(void);

size_t get_internal_cmd_list_size(void);

void load_external_commands(void);

void capture_system_call(struct cmd_return *ret, String command);

#endif /* SHELL_CMD_H */
