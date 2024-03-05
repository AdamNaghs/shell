#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"

int main(void)
{
    /*
    for (int i = 0; i < 10; i++)
        test_creds();
    */
    shell_loop();
    return 0;
}
