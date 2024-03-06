/* Using WSL Ubuntu 
    |  afl-clang-fast -g -fsanitize=address,undefined fuzz_test.c 
    |  afl-fuzz -i inputs/ -o results ./a.out*/
#include "include/string_array.h"
#include "include/shell.h"
#include "include/string.h"
#include "include/IO.h"
#include "include/shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

__AFL_FUZZ_INIT();

int main(void)
{
    __AFL_INIT();
    char *src = 0;
    unsigned char *buf = __AFL_FUZZ_TESTCASE_BUF;
    while (__AFL_LOOP(10000)) {
        int len = __AFL_FUZZ_TESTCASE_LEN;
        src = realloc(src, len+1);
        memcpy(src, buf, len);
        src[len] = 0;
        String str = str_new(src);
        shell_loop_manual_step(&str, false, false, false);
        str_free(str);
    }
}