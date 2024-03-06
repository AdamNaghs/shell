/* Using WSL Ubuntu 
    |  afl-clang-fast -g -fsanitize=address,undefined fuzz_test.c 
    |  afl-fuzz -i inputs/ -o results ./a.out*/
#include "src/string_array.c"
#include "src/string.c"
#include "src/IO.c"
#include <unistd.h>

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
        String_Array a = str_split(STR(src), STR(" "));
        str_arr_free(a);
    }
}