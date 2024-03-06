#ifndef SHELL_TOKENIZE
#define SHELL_TOKENIZE
#include "string.h"
#include "string_array.h"

typedef enum token_type
{
    NONE,
    STRING,
    PUNCT,
}Token_Type;

typedef struct token
{
    String str;
    enum token_type type;
} Token;

typedef struct token_array
{
    Token *arr;
    size_t size;
} Token_Array;

/*
    Tokenizing Rule:
        1. Leading and trailing whitespace is ignored
        2. whitespace is any of \n, \r, \t, ' ',
        3. All punctuation character are tokens and delimiters/seperators
            i.e (){}:;|*!@#$%^&*()_+-=[]'",<>/?\|
        4. Strings are meant to be parsed one line at a time so multiple lines may need to be parsed before code can be ran.
            ex: for an 'ifeq' command the first line would take
            "ifeq arg1 arg2:" (this would be 5 tokens)
            then the if command will continue reading until it recieves a semi-colon or
            "else" followed by a semi colon
        5. When quotes are around anything, that portion is not segmented by spaces


*/

/* returns heap allocated array of tokens */
Token_Array tokenize(String line);

void free_token_array(Token_Array);

bool has_minimum_tokens(Token_Type* expected, size_t expected_size, Token_Array given);

#endif /* SHELL_TOKENIZE */