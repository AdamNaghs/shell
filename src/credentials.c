#include "../include/credentials.h"
#include "../include/IO.h"
#include "../include/utils.h"
#include "../include/string.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct 
{
    String operation;
    String readable;
} Big_Int;

Big_Int big_int_init(String base)
{
    Big_Int bi = {str_new(base.cstr),str_new(base.cstr)};
    str_reverse(&bi.operation);
    return bi;
}

void big_int_free(Big_Int bi)
{
    str_free(bi.operation);
    str_free(bi.readable);
}

/* string does not need to be freed, it belongs to the Big_Int */
String big_int_read(Big_Int* bi)
{
    str_free(bi->readable);
    bi->readable = str_new(bi->operation.cstr);
    str_reverse(&bi->readable);
    return bi->readable;
}

#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)
/* only worry about the operation string */
Big_Int big_int_add(Big_Int a, Big_Int b) {
    Big_Int result = big_int_init(STR(""));
    int carry = 0;
    size_t max_size = MAX(a.operation.size, b.operation.size);

    for (size_t i = 0; i < max_size; i++) {
        int digit_a = (i < a.operation.size) ? a.operation.cstr[i] - '0' : 0;
        int digit_b = (i < b.operation.size) ? b.operation.cstr[i] - '0' : 0;
        int sum = digit_a + digit_b + carry;
        carry = sum / 10;
        sum %= 10;

        char digit_str[2] = {sum + '0', '\0'};
        str_append(&result.operation, STR(digit_str));
    }

    if (carry > 0) {
        char carry_str[2] = {carry + '0', '\0'};
        str_append(&result.operation, STR(carry_str));
    }

    return result;
}

Big_Int big_int_sub(Big_Int a, Big_Int b) {
    Big_Int result = big_int_init(STR(""));
    int carry = 0;
    size_t max_size = MAX(a.operation.size, b.operation.size);

    for (size_t i = 0; i < max_size; i++) {
        int digit_a = (i < a.operation.size) ? a.operation.cstr[i] - '0' : 0;
        int digit_b = (i < b.operation.size) ? b.operation.cstr[i] - '0' : 0;
        int sum = digit_a - digit_b - carry;
        carry = sum / 10;
        sum %= 10;

        char digit_str[2] = {sum + '0', '\0'};
        str_append(&result.operation, STR(digit_str));
    }

    if (carry > 0) {
        char carry_str[2] = {carry + '0', '\0'};
        str_append(&result.operation, STR(carry_str));
    }

    return result;
}

Big_Int big_int_mult(Big_Int a, Big_Int b) {
    Big_Int result = big_int_init(STR("0"));

    for (size_t i = 0; i < a.operation.size; i++) {
        int carry = 0;
        String temp = str_new(NULL);

        // Pad with leading zeros for the current digit
        for (size_t j = 0; j < i; j++) {
            str_append(&temp, STR("0"));
        }

        int digit_a = a.operation.cstr[i] - '0';
        for (size_t j = 0; j < b.operation.size; j++) {
            int digit_b = b.operation.cstr[j] - '0';
            int product = digit_a * digit_b + carry;
            carry = product / 10;
            product %= 10;

            char digit_str[2] = {product + '0', '\0'};
            str_append(&temp, STR(digit_str));
        }

        if (carry > 0) {
            char carry_str[2] = {carry + '0', '\0'};
            str_append(&temp, STR(carry_str));
        }
        // Reverse the intermediate result
        str_reverse(&temp);
        Big_Int tmp_big_int = big_int_init(temp);
        // Add the intermediate result to the final result
        result = big_int_add(result, tmp_big_int);
        big_int_free(tmp_big_int);
        str_free(temp);
    }
    return result;
}


Big_Int big_int_div(Big_Int a, Big_Int b) {
    Big_Int result = big_int_init(STR("0"));

    for (size_t i = 0; i < a.operation.size; i++) {
        int carry = 0;
        String temp = str_new(NULL);

        // Pad with leading zeros for the current digit
        for (size_t j = 0; j < i; j++) {
            str_append(&temp, STR("0"));
        }

        int digit_a = a.operation.cstr[i] - '0';
        for (size_t j = 0; j < b.operation.size; j++) {
            int digit_b = b.operation.cstr[j] - '0';
            int divisor = digit_a / digit_b - carry;
            carry = divisor / 10;
            divisor %= 10;

            char digit_str[2] = {divisor + '0', '\0'};
            str_append(&temp, STR(digit_str));
        }

        if (carry > 0) {
            char carry_str[2] = {carry + '0', '\0'};
            str_append(&temp, STR(carry_str));
        }
        // Reverse the intermediate result
        str_reverse(&temp);
        Big_Int tmp_big_int = big_int_init(temp);
        // Add the intermediate result to the final result
        result = big_int_add(result, tmp_big_int);
        big_int_free(tmp_big_int);
        str_free(temp);
    }
    return result;
}


Big_Int big_int_mod(Big_Int a, Big_Int b) {
    Big_Int result = big_int_init(STR("0"));

    for (size_t i = 0; i < a.operation.size; i++) {
        int carry = 0;
        String temp = str_new(NULL);

        // Pad with leading zeros for the current digit
        for (size_t j = 0; j < i; j++) {
            str_append(&temp, STR("0"));
        }

        int digit_a = a.operation.cstr[i] - '0';
        for (size_t j = 0; j < b.operation.size; j++) {
            int digit_b = b.operation.cstr[j] - '0';
            int divisor = digit_a % digit_b - carry;
            carry = divisor / 10;
            divisor %= 10;

            char digit_str[2] = {divisor + '0', '\0'};
            str_append(&temp, STR(digit_str));
        }

        if (carry > 0) {
            char carry_str[2] = {carry + '0', '\0'};
            str_append(&temp, STR(carry_str));
        }
        // Reverse the intermediate result
        str_reverse(&temp);
        Big_Int tmp_big_int = big_int_init(temp);
        // Add the intermediate result to the final result
        result = big_int_add(result, tmp_big_int);
        big_int_free(tmp_big_int);
        str_free(temp);
    }
    return result;
}

bool big_int_eq(Big_Int a, Big_Int b)
{
    size_t max = MAX(a.operation.size,b.operation.size);
    size_t i;
    for (i = 0; i < max; i++)
    {
        char ac = i < a.operation.size ? a.operation.cstr[i] : 0;
        char bc = i < b.operation.size ? b.operation.cstr[i] : 0;
        if (ac != bc)
            return false;
    }
    return true;
}


Big_Int str_hash(String string)
{
    char *str = string.cstr;
    size_t len = string.size;
    size_t i;
    Big_Int hash_value = big_int_init(STR_LIT("7"));
    Big_Int bi_p_pow = big_int_init(STR_LIT("1"));
    Big_Int bi_p = big_int_init(STR_LIT("31"));
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        char buf[6];
        snprintf(buf,10,"%d",(int)c);
        Big_Int bi_c = (Big_Int){STR(buf)};
        //hash_value = (hash_value + (c - 'a' + 1) * p_pow);
        Big_Int bi_hash_value = big_int_add(hash_value,bi_c);
        big_int_free(hash_value);
        hash_value = bi_hash_value;

        Big_Int bi_hash_value_time_p_pow = big_int_mult(hash_value,bi_p_pow);
        big_int_free(hash_value);
        hash_value = bi_hash_value_time_p_pow;
        //p_pow = (p_pow * p);
        Big_Int tmp_p_pow =  big_int_mult(bi_p_pow,bi_p);
        big_int_free(bi_p_pow);
        bi_p_pow = tmp_p_pow;
    }
    return hash_value;
}

#define SALT_MAX 50
#define SALT_MIN 25
String generate_salt(void)
{
    static size_t count = 0;
    srand(time(NULL) + count);

    String ret = {.cstr = (char *)malloc(SALT_MAX * sizeof(char) + 1), .size = rand() % SALT_MAX};
    if (ret.size < SALT_MIN)
    {
        ret.size += SALT_MIN;
    }
    if (!ret.cstr)
    {
        // Handle malloc failure
        return ret;
    }

    for (size_t i = 0; i < ret.size; i++)
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
    ret.cstr[ret.size] = '\0';
    count++;
    return ret;
}

#define HASH_MAX 2000
Big_Int generate_hash(String str, String salt)
{
    String tmp = str_new(str.cstr);
    str_append(&tmp, salt);
    Big_Int ret = str_hash(tmp);
    str_free(tmp);
    return ret;
}

#define CREDS_FILE_PATH ".asn_users"
bool check_login(String username, String password);

void new_user(void)
{
    FILE *f = FOPEN(CREDS_FILE_PATH, "a");
    if (!f)
    {
        perror("Could not open password file.\n");
        exit(1);
    }
    enable_input_buffer_display();
    printf("Username: ");
    String username = input('\n', 0);
    printf("\nPassword: ");
    disable_input_buffer_display();
    String password = input('\n', 0);
    enable_input_buffer_display();
    String salt = generate_salt();
    Big_Int hash = generate_hash(password, salt);
    FILE* tmp = get_output_file();
    set_output_file(f);
    output( "%s asn1|%s$%s\n", username.cstr, big_int_read(&hash).cstr, salt.cstr);
    set_output_file(tmp);
    FCLOSE(f);
    if (!check_login(username, password))
    {
        perror("Fatal error: could not find user which was just created.\n");
        //remove(CREDS_FILE_PATH);
        exit(1);
    }
    ;
    str_free(salt);
    str_free(username);
    str_free(password);
    big_int_free(hash);
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
        String salt = {salt_start + 1, line.cstr + line.size - salt_start - 1};
        Big_Int test_hash = generate_hash(password, salt);
        *salt_start = '\0';
        String file_hash_str = str_new(pipe+1);
        Big_Int file_hash = big_int_init(file_hash_str);
        if (big_int_eq(test_hash,file_hash))
        {
            printf("User found!\n");
            found = true;
        }
        big_int_free(test_hash);
        big_int_free(file_hash);

    next_line:
        str_free(line);
    }

    set_input_file(tmp);
    FCLOSE(f);
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