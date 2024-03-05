#include "../include/utils.h"
#include <stdbool.h>
#ifdef _WIN32
bool is_dir(char* filename)
{

}
#else
#include <sys/stat.h>
bool is_dir(char* filename)
{
    struct stat file_stat;
    stat(filename, &file_stat);
    return S_ISDIR(file_stat.st_mode);
}
#endif
