#include "../include/string.h"
#include "../include/IO.h"
#include <stdlib.h>

#define MAX_STR 10000

static FILE *input_file = NULL;

void set_input_file(FILE *fd)
{
    if (!fd)
    {
        perror(RED "'fd' is null.\n" CRESET);
        exit(1);
    }
    input_file = fd;
}

static char c;

bool at_eof(void)
{
    return c == EOF;
}

String input(char enter_char, size_t max_size)
{
    if (!input_file)
    {
        input_file = stdin;
    }
    String ret = {.cstr = NULL, .size = 0};
    if (max_size == 0)
    {
        ret.cstr = (char *)malloc(MAX_STR * sizeof(char));
        max_size = MAX_STR - 1;
    }
    else
    {
        ret.cstr = (char *)malloc(max_size * sizeof(char) + 1);
    }
    if (!ret.cstr)
    {
        perror(RED "'input' could not malloc ret.cstr.\n" CRESET);
        exit(1);
    }
    c = ' ';
    while ((c = fgetc(input_file)) != enter_char && ret.size < max_size)
    {
        if (c == EOF || c == '\0')
            break;
        ret.cstr[ret.size++] = c;
    }

    ret.cstr = (char *)realloc(ret.cstr, ret.size + 1);
    if (!ret.cstr)
    {
        perror(RED "'input' could not realloc ret.cstr.\n" CRESET);
        exit(1);
    }
    ret.cstr[ret.size] = '\0';
    return ret;
}

