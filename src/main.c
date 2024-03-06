#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"
#include "../include/utils.h"
#include "../include/string_array.h"
#include <time.h>
#include <stdlib.h>
void test_shell_loop(void)
{
    clock_t start = clock();
    char mode[2] = "r";
    #ifdef _WIN32
    char path[49] = "C:\\Users\\adamn\\Dropbox\\src\\c code\\shell\\test.asn";
    #else
    char path[9] = "test.asn";
    #endif
    FILE *f = FOPEN(path, mode);
    set_input_file(f);
    shell_loop_test();
    set_input_file(stdin);
    FCLOSE(f);
    printf("\nTest File Runtime: %lums\n", clock() - start);
}

void test_str_split(void)
{
    String_Array arr;
    String case0 = STR(" ");
    arr = str_split(case0, STR(" "));
    str_arr_free(arr);
    String case1 = STR("  ");
    arr = str_split(case1, STR(" "));
    str_arr_free(arr);
    String case2 = STR(" ");
    arr = str_split(case2, STR(" "));
    str_arr_free(arr);
}

int main1(void)
{
    /*
    for (int i = 0; i < 10; i++)
        test_creds();
    */
    test_str_split();
    test_shell_loop();
    shell_loop();
    return 0;
}
