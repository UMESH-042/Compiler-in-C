#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Token types

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_STRING,
    TOKEN_PREPROCESSOR,
    TOKEN_HEADER,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
    int line;
} Token;

// Function declarations
void init_lexer(const char *filename);
Token get_next_token();
void close_lexer();

#endif // LEXER_H
