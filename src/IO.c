#include "../include/string.h"
#include "../include/IO.h"
#include <stdlib.h>

#define MAX_STR 10000

FILE *input_file = NULL;

void set_input_file(FILE *fd)
{
    if (!fd)
    {
        perror(RED "'fd' is null.\n" CRESET);
        exit(1);
    }
    input_file = fd;
}

FILE* get_input_file(void)
{
    if (!input_file)
        return stdout;
    return input_file;
}

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void disable_input_buffer_display()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
#else
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}

void enable_input_buffer_display()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
#else
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
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

