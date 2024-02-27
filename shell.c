#include "include\shell.h"
#include "include\IO.h"
#include "include\signal.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 1000

String_Array sys_call(String s)
{
    FILE *p = _popen(s.cstr, "r");
    if (!p)
        return (String_Array){.arr = NULL, .size = 0};
    String_Array ret = {.arr = (String *)malloc(10 * sizeof(String)), .size = 10};
    size_t i = 0;
    while (1)
    {
        char *line;
        char buf[MAX_LINE_WIDTH];
        line = fgets(buf, sizeof(buf), p);
        if (line == NULL)
            break;
        printf("%s", line); /* line includes '\n' */
        size_t len = strlen(buf);
        if (i >= ret.size)
        {
            ret.arr = (String *)realloc(ret.arr, ret.size *= 2);
            if (!ret.arr)
            {
                perror(RED "'sys_call' could not realloc ret.arr.\n" CRESET);
                exit(1);
            }
        }
        ret.arr[ret.size++] = (String){.cstr = (char *)malloc(len * sizeof(char) + 1), .size = len};
        i++;
    }
    _pclose(p);
    return ret;
}

void shell_loop(void)
{
    bind_signals();
    while (1)
    {
        printf(BLU "asn@tmpUser> " CRESET);
        String a = input('\n', 0);
        sys_call(a);
        str_free(a);
    }
}