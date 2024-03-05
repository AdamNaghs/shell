#ifndef SHELL_UTILS
#define SHELL_UTILS
#include <stdbool.h>


#ifdef _WIN32
#include <direct.h>
#define GETCWD _getcwd
#define CHDIR _chdir
#define MKDIR _mkdir
#define RMDIR _rmdir
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#define GETCWD getcwd
#define CHDIR chdir
int MKDIR(char *path);
#define RMDIR rmdir
#endif

bool is_dir(char* str);

#endif
