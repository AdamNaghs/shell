#include "../include/credentials.h"
#include "../include/IO.h"
#include <stdlib.h>
#include <time.h>

#define SALT_LEN 10

String generate_salt(void)
{
    static size_t count = 0;
    srand(time(NULL) + count);
    size_t i = 0;
    String ret = {.cstr = (char *)malloc(SALT_LEN * sizeof(char) + 1), .size = SALT_LEN};
    for (; i < SALT_LEN; i++)
    {
        ret.cstr[i] = 48 + rand() % (126 - 48); /* ASCII range 48 to 126 */
    }
    ret.cstr[SALT_LEN] = '\0';
    count++;
    return ret;
}

#define HASH_LEN 10

String generate_hash(String str)
{
    String ret = {.cstr = (char *)malloc(HASH_LEN * sizeof(char) + 1), .size = HASH_LEN};
    size_t i = 0;
    for (; i < ret.size; i++)
    {
        ret.cstr[i] =( str_hash(str) % (i + 1)) + 1;
    }
    ret.cstr[ret.size] = '\0';
    return ret;
}

void test_creds(void)
{
    printf("Testing 'generate_salt.\n'");
    String salt = generate_salt();
    printf("Salt: %s\n", salt.cstr);
    str_free(salt);

    printf("Testing 'generate_hash' on str 'test'.\n");
    char test_char[5] = "test";
    String test_str = str_new(test_char);
    String hash = generate_hash(test_str);
    printf("Hash: %s\n",hash.cstr);
    str_free(test_str);
    str_free(hash);

}

size_t str_hash(String string)
{
    char *str = string.cstr;
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