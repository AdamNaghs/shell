#include "../include/utils.h"
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
bool is_dir(char* filename)
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
#else
#include <sys/stat.h>
bool is_dir(char* filename)
{
    struct stat file_stat;
    stat(filename, &file_stat);
    return S_ISDIR(file_stat.st_mode);
}
#endif
