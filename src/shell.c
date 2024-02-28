#include "../include/shell.h"
#include "../include/IO.h"
#include "../include/signal.h"
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
String_Array sys_call(String s)
{
    FILE *p = popen(s.cstr, "r");
    if (!p)
        return (String_Array){.arr = NULL, .size = 0};
    String_Array ret = {.arr = (String *)malloc(10 * sizeof(String)), .size = 10};
    size_t i = 0;
    while (1)
    {
        char *line;
        char buf[MAX_LINE_WIDTH];
        line = fgets(buf, MAX_LINE_WIDTH, p);
        if (line == NULL)
        {
            break;
        }
        printf("%s", line); /* line includes '\n' */
        size_t len = strnlen(line,MAX_LINE_WIDTH);
        if (i >= ret.size)
        {
            String* tmp = (String *)realloc(ret.arr, ret.size *= 2);
            if (!tmp)
            {
                perror(RED "'sys_call' could not realloc ret.arr.\n" CRESET);
                pclose(p);
                exit(1);
            }
            ret.arr = tmp;
        }
        ret.arr[i] = (String){.cstr = (char *)malloc(len * sizeof(char) + 1), .size = len};
        memcpy_s(ret.arr[i].cstr,len+1,buf,len);
        ret.arr[i].cstr[len] = '\0'; 
        i++;
    }
    ret.size = i;
    if (-1 == pclose(p))
    {
        perror(RED "'sys_call' failed to close file.\n");
        printf("Code %d\n", errno);
        str_arr_free(ret);
        exit(1);
    }
    String* tmp = (String *)realloc(ret.arr, ret.size * sizeof(String));
    if (i && !tmp)
    {
        perror(RED "'sys_call' could not realloc ret.arr.\n" CRESET);
        exit(1);
    }
    ret.arr = tmp;
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