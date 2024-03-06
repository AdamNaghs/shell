#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"
#include "../include/utils.h"
#include "../include/string_array.h"
#include "../include/tokenize.h"
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
char path[49] = "C:\\Users\\adamn\\Dropbox\\src\\c code\\shell\\test.asn";
#else
char path[53] = "/mnt/c/Users/adamn/Dropbox/src/c code/shell/test.asn";
#endif
void test_shell_loop(void)
{
    clock_t start = clock();
    char mode[2] = "r";
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

void test_tokenize(void)
{
    FILE *fd = FOPEN(path, "r");
    FILE *tmp_file = get_input_file();
    set_input_file(fd);
    while (!at_eof())
    {
        String s = input('\n', 0);
        Token_Array ta = tokenize(s);
        size_t i = 0;
        for (; i < ta.size; i++)
            printf("%s ", ta.arr[i].str.cstr);
        printf("\nSize: %d\n", ta.size);
        free_token_array(ta);
        str_free(s);
    }
    set_input_file(tmp_file);
    exit(0);
}

int main(void)
{
    /*
    for (int i = 0; i < 10; i++)
        test_creds();
    */
    // test_str_split();
    // for (size_t i = 0; i < 10000; i++)
    //     test_shell_loop();
    // test_tokenize();k
    test_shell_loop();
    shell_loop();
    return 0;
}
