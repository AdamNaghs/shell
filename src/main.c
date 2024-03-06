#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"
#include "../include/utils.h"
void test_shell_loop(void)
{
    char mode[2] = "r";
    char path[9] = "Test.txt";
    FILE *f = FOPEN(path, mode);
    set_input_file(f);
    while (!at_eof())
        shell_loop_step(true);
    set_input_file(stdin);
    FCLOSE(f);
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
