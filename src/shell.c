#include "../include/shell.h"
#include "../include/IO.h"
#include "../include/signal.h"
#include "../include/cmd.h"
#include "../include/builtins.h"
#include "../include/credentials.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_LINE_WIDTH 1000

#ifndef popen
#define popen _popen
#endif
#ifndef pclose
#define pclose _pclose
#endif
#ifndef strdup
#define strdup _strdup
#endif


void shell_loop(void)
{
    bind_signals();
    attempt_login_loop();
    load_builtins();
    char buf[2] = " ";
    String delim = (String){.cstr = buf, .size = 1};
    struct internal_cmd* cmd_list = get_internal_cmd_list();

/*
    const char* osys_char_ptr = "osys";
    add_internal_cmd(internal_cmd_new(str_new(osys_char_ptr),outer_sys_call));
*/
    while (1)
    {
        printf(BLU "asn@tmpUser> " CRESET);
        String a = input('\n', 0);
        if (a.size == 0)
            continue;
        String_Array arr = str_split(a,delim);
        size_t i = 0;
        bool ran = false;
        for (; i < get_internal_cmd_list_size(); i++)
        {
            if (0 == strcmp(arr.arr[0].cstr, cmd_list[i].name.cstr))
            {
                cmd_list[i].func(arr);
                ran = true;
                break;
            }
        }
        str_free(a);
        if (!ran)
        {
            printf("Could not find command '%s'.\n",arr.arr[0].cstr);
        }
        str_arr_free(arr);
    }
}