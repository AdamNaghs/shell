#include "../include/tokenize.h"
#include "../include/colors.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
void tok_arr_add(Token_Array *ta, size_t *ta_cap, Token tok)
{
    if (ta->size >= *ta_cap)
    {
        *ta_cap *= 2;
        Token *tmp = (Token *)realloc(ta->arr, *ta_cap * sizeof(Token));
        if (!tmp)
        {
            perror(RED "asn: Could not realloc token array.\n" CRESET);
            exit(1);
        }
        ta->arr = tmp;
    }
    ta->arr[ta->size++] = tok;
}

Token_Array tokenize1(String line)
{
    size_t cap = 10;
    Token_Array ret = {.arr = (Token *)malloc(cap * sizeof(Token)), .size = 0};

    size_t start_idx = 0;  // Start index of a token
    bool in_token = false; // Are we currently in a token

    for (size_t i = 0; i <= line.size; i++)
    {
        if (isspace(line.cstr[i]) || ispunct(line.cstr[i]) || i == line.size)
        {
            if (in_token)
            {
                // End of the current token
                size_t token_length = i - start_idx;
                String s = str_new_n(line.cstr + start_idx, token_length);

                Token t = {.str = s, .type = STRING}; // Assuming WORD is a token type
                tok_arr_add(&ret, &cap, t);

                in_token = false;
            }

            if (ispunct(line.cstr[i]))
            {
                // It's a punctuation character
                char tmp[2] = {line.cstr[i], '\0'};
                Token t = {.str = str_new(tmp), .type = PUNCT};
                tok_arr_add(&ret, &cap, t);
            }
        }
        else if (!in_token)
        {
            // Start of a new token
            start_idx = i;
            in_token = true;
        }
    }

    return ret;
}

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Assuming Token, Token_Array, and other relevant structures and enums are defined

Token_Array tokenize(String line)
{
    size_t cap = 10;
    Token_Array ret = {.arr = (Token *)malloc(cap * sizeof(Token)), .size = 0};
    size_t start_idx = 0;
    bool in_token = false;
    bool in_quotes = false; // New flag to check if we are inside quotes

    for (size_t i = 0; i <= line.size; i++)
    {
        // Check for start or end of quotes
        if (line.cstr[i] == '"' && (i == 0 || line.cstr[i - 1] != '\\'))
        {
            if (!in_quotes && !in_token)
            {
                // Starting a quoted token
                start_idx = i + 1;
                in_quotes = true;
                continue;
            }
            else if (in_quotes)
            {
                // Ending a quoted token
                size_t token_length = i - start_idx;
                String s = str_new_n(line.cstr + start_idx, token_length);
                Token t = {.str = s, .type = STRING}; // Assuming QUOTED is a token type
                tok_arr_add(&ret, &cap, t);

                in_quotes = false;
                i++; // Skip processing the closing quote character
            }
        }

        if (!in_quotes)
        {
            if (isspace(line.cstr[i]) || ispunct(line.cstr[i]) || i == line.size)
            {
                if (in_token)
                {
                    // End of the current token
                    size_t token_length = i - start_idx;
                    String s = str_new_n(line.cstr + start_idx, token_length);
                    Token t = {.str = s, .type = STRING};
                    tok_arr_add(&ret, &cap, t);

                    in_token = false;
                }

                if (ispunct(line.cstr[i]))
                {
                    // Handle punctuation
                    char tmp[2] = {line.cstr[i], '\0'};
                    Token t = {.str = str_new(tmp), .type = PUNCT};
                    tok_arr_add(&ret, &cap, t);
                }
            }
            else if (!in_token)
            {
                // Start of a new token
                start_idx = i;
                in_token = true;
            }
        }
    }

    return ret;
}

void free_token_array(Token_Array ta)
{
    size_t i;
    for (i = 0; i < ta.size; i++)
    {
        str_free(ta.arr[i].str);
    }
}