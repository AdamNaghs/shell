#include "include/IO.h"
#include "include/string.h"
#include "include/shell.h"
int main()
{
    String str = input('\n',0);
    char delim[1] = {' '};
    String_Array a = str_split(str,(String){delim,1});
    printf("%s\n",str.cstr);
    for (size_t i = 0; i <a.size; i ++)
    {
        printf("%s\n",a.arr[i].cstr);
    }
    str_free(str);
    str_arr_free(a);

    shell_loop();
    return 0;
}