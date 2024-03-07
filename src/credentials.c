#include "../include/credentials.h"
#include "../include/IO.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SALT_LEN 20

String generate_salt(void)
{
    static size_t count = 0;
    srand(time(NULL) + count);

    String ret = {.cstr = (char *)malloc(SALT_LEN * sizeof(char) + 1), .size = SALT_LEN};
    if (!ret.cstr)
    {
        // Handle malloc failure
        return ret;
    }

    for (size_t i = 0; i < SALT_LEN; i++)
    {
        int randomValue = rand() % 62; // There are 26 lowercase, 26 uppercase letters, and 10 digits
        if (randomValue < 10)
        {
            // 0-9
            ret.cstr[i] = '0' + randomValue;
        }
        else if (randomValue < 36)
        {
            // A-Z
            ret.cstr[i] = 'A' + (randomValue - 10);
        }
        else
        {
            // a-z
            ret.cstr[i] = 'a' + (randomValue - 36);
        }
    }
    ret.cstr[SALT_LEN] = '\0';
    count++;
    return ret;
}

#define HASH_MAX 2000
size_t generate_hash(String str, String salt)
{
    String tmp = str_new(str.cstr);
    str_append(&tmp, salt);
    size_t ret = str_hash(tmp);
    str_free(tmp);
    return ret;
}

size_t str_hash(String string)
{
    char *str = string.cstr;
    size_t len = string.size;
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    size_t i;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
/* String get_username(void) */

/* String get_password(void) */
#define CREDS_FILE_PATH ".asn_users"
bool check_login(String username, String password);

void new_user(void)
{
    printf("Username: ");
    String username = input('\n', 0);
    printf("\nPassword: ");
    disable_input_buffer_display();
    String password = input('\n', 0);
    enable_input_buffer_display();
    FILE *f = FOPEN(CREDS_FILE_PATH, "a");
    if (!f)
    {
        perror("Could not open password file.\n");
        exit(1);
    }
    String salt = generate_salt();
    size_t hash = generate_hash(username, salt);
    fprintf(f, "%s asn0|%llu$%s\n", username.cstr, hash, salt.cstr);
    FCLOSE(f);
    check_login(username, password);
}

bool check_login(String username, String password)
{
    FILE *f = FOPEN(CREDS_FILE_PATH, "r");
    if (!f)
    {
        perror("Could not open password file.\n");
        exit(1);
    }
    FILE *tmp = get_input_file();
    set_input_file(f);
    bool found = false;
    while (!at_eof() && !found)
    {
        String line = input('\n', 0);
        char *sep = strchr(line.cstr, ' ');
        if (!sep)
        {
            goto next_line;
        }
        String name = {line.cstr, sep - line.cstr};
        if (!str_equal(name, username))
        {
            goto next_line;
        }
        char *pipe = strchr(sep + 1, '|');
        if (!pipe)
        {
            goto next_line;
        }
        char *salt_start = strchr(pipe + 1, '$');
        if (!salt_start)
        {
            goto next_line;
        }
        String salt = {salt_start + 1, SALT_LEN};
        size_t test_hash = generate_hash(password, salt);
        size_t file_hash = strtoull(pipe + 1, NULL, 10);
        if (test_hash == file_hash)
        {
            printf("User found!\n");
            found = true;
        }

    next_line:
        str_free(line);
    }

    set_input_file(tmp);
    return found;
}

void test_creds(void)
{
    // printf("Testing 'generate_salt.\n'");
    // String salt = generate_salt();
    // printf("Salt: %s\n", salt.cstr);
    // printf("Testing 'generate_hash' on str 'test'.\n");
    // size_t hash = generate_hash(STR_LIT("Test String super long password asdasdasjdasdnaksfdgw23135350sdasd"), salt);
    // str_free(salt);
    // char buf[10000];
    // snprintf(buf, 100, "%zx", hash);
    // printf("Hash: %zu, In Hex: %s\n", hash, buf);
    new_user();
}

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

int file_exists(const char *filename)
{
#ifdef _WIN32
    return _access(filename, 0) == 0;
#else
    return access(filename, F_OK) != -1;
#endif
}


void attempt_login_loop(void)
{
    if (!file_exists(CREDS_FILE_PATH))
    {
        printf("Please create a login.\n");
        new_user();
        return;
    }
    bool found = false;
    while (!found)
    {
        printf("Username: ");
        String username = input('\n', 0);
        printf("Password: ");
        disable_input_buffer_display();
        String password = input('\n', 0);
        if (check_login(username, password))
            found = true;
        else
            printf("\nCould not find a user matching that username and password.\n");
        enable_input_buffer_display();
        str_free(username);
        str_free(password);
    }
}