#include "../include/credentials.h"
#include "../include/IO.h"



size_t str_hash(String string)
{
    char* str = string.cstr;
    size_t len = string.size;
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (size_t i = 0; i < len; i++)
    {
        char c = str[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
/* String get_username(void) */

/* String get_password(void) */

/* returns -1 on fail and 0 on success */
void attempt_login_loop()
{
    return;
}