#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"
#include "../include/utils.h"
#include <time.h>
void test_shell_loop(void)
{
    clock_t start = clock();
    char mode[2] = "r";
    char path[9] = "test.asn";
    FILE *f = FOPEN(path, mode);
    set_input_file(f);
    shell_loop_test();
    set_input_file(stdin);
    FCLOSE(f);
    printf("\nTest File Runtime: %lums\n",clock()-start);
}

int main(void)
{
    /*
    for (int i = 0; i < 10; i++)
        test_creds();
    */
    test_shell_loop();
    shell_loop();
    return 0;
}
