#ifndef SHELL_UTILS
#define SHELL_UTILS
#include <stdbool.h>


#ifdef _WIN32
#include <direct.h>
#include <stdio.h>
#define GETCWD _getcwd
#define CHDIR _chdir
#define MKDIR _mkdir
#define RMDIR _rmdir
#define POPEN _popen
#define PCLOSE _pclose
FILE* FOPEN(char *path,char* mode);
#define FCLOSE fclose
#else
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#define GETCWD getcwd
#define CHDIR chdir
int MKDIR(char *path);
#define RMDIR rmdir
#define POPEN popen
#define PCLOSE pclose
#define FOPEN fopen
#define FCLOSE fclose
#define GETENV getenv
#endif

bool is_dir(char* str);

#endif
