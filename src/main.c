#include "../include/IO.h"
#include "../include/string.h"
#include "../include/shell.h"
#include "../include/credentials.h"

int main()
{
    /*
    String str = input('\n',0);
    char delim[1] = {' '};
    String_Array a = str_split(str,(String){delim,1});

    for (size_t i = 0; i <a.size; i ++)
    {
        printf("%s\n",a.arr[i].cstr);
    }
    str_free(str);
    str = str_arr_join(a, ' ');
    printf("%s\n",str.cstr);
    str_arr_free(a);
    */
    for (int i = 0; i < 10; i++)
        test_creds();
    shell_loop();
    return 0;
}