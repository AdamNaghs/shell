#include "../include/utils.h"
#include "../include/colors.h"
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <errno.h>
#include <stdlib.h>
/* currently unused in windows implementation of the shell */
bool is_dir(char *filename)
{
    DWORD file_attr = GetFileAttributes(filename);
    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}
FILE *FOPEN(char *path, char *mode)
{
    FILE *f = (FILE *)malloc(sizeof(FILE));
    if (0 != fopen_s(&f, path, mode))
    {
        perror(RED"'FOPEN' could not open file.\n"CRESET);
        exit(1);
    }

    return f;
}
#define MAX_ENV 16384
char env_buf[16384];
char *GETENV(char *path)
{
    /*size_t size = MAX_ENV;
    _dupenv_s(&env_buf,&size,path);
    return env_buf;*/
    return getenv(path);
}
#else
#include <sys/stat.h>
bool is_dir(char *filename)
{
    struct stat file_stat;
    stat(filename, &file_stat);
    return S_ISDIR(file_stat.st_mode);
}
int MKDIR(char *path)
{
    return mkdir(path, 777);
}
#endif
