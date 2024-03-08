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
    String case0 = STR_LIT(" ");
    arr = str_split(case0, STR_LIT(" "));
    str_arr_free(arr);
    String case1 = STR_LIT("  ");
    arr = str_split(case1, STR_LIT(" "));
    str_arr_free(arr);
    String case2 = STR_LIT(" ");
    arr = str_split(case2, STR_LIT(" "));
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
#define LOOP_MAX 100000 /* 1 000 000 000*/

void test_string_speed(void)
{
    static int batch = 0;
    clock_t start = clock();
    String str = str_new(NULL);
    size_t i;
    printf("Starting String Speed Batch %d with %d iterations.\n", batch, LOOP_MAX);
    for (i = 0; i < LOOP_MAX; i++)
    {
        str_append(&str, STR_LIT("A not very but still reasonably long string.\n"));
    }
    str_free(str);
    printf("String_Append TEST%d ran in %lfms \n", 1 + batch, ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000);

    start = clock();
    for (i = 0; i < LOOP_MAX; i++)
    {
        str = str_new(NULL);
        str_append(&str, STR_LIT("A not very but still reasonably long string.\n"));
        str_free(str);
    }
    printf("String_Append TEST%d ran in %lfms \n", 2 + batch, ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000);

    start = clock();
    str = STR_LIT("A not very but still reasonably long string.\n");
    String memcpy_str = (String){(char*)malloc(LOOP_MAX * str.size + 1), 0};
    for (i = 0; i < (LOOP_MAX * str.size); i+=str.size)
    {
        str_memcpy(&memcpy_str,i,str);
    }
    printf("String_Memcpy TEST%d ran in %lfms \n", 1 + batch, ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000);

    start = clock();
    str = STR_LIT("A not very but still reasonably long string.\n");
    for (i = 0; i < LOOP_MAX; i++)
    {
        Token_Array ta = tokenize(str);
        free_token_array(ta);
    }
    printf("Token_Array TEST%d ran in %lfms \n", 1 + batch, ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000);
    start = clock();
    str = STR_LIT("A not very but still reasonably long string.\n");
    Token_Array ta = tokenize(str);
    for (i = 0; i < LOOP_MAX; i++)
    {
        String s = token_array_to_str(ta, ' ');
        str_free(s);
    }
    free_token_array(ta);
    printf("Token_Array to str TEST%d ran in %lfms\n", 1 + batch, ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000);
    batch++;
    printf("\n");
}

int main(void)
{

    // for (int i = 0; i < 10; i++)
    //     test_creds();

    // test_str_split();
    // for (size_t i = 0; i < 10000; i++)
    //     test_shell_loop();
    // test_tokenize();k
    // test_shell_loop();
    // for (int i = 0; i < 10; i++)
    //     test_string_speed();
    shell_loop();
    return 0;
}
