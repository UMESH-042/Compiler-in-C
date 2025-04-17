#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Token types

typedef enum
{
    // General
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_STRING,
    TOKEN_CHAR_LITERAL,
    TOKEN_PREPROCESSOR,
    TOKEN_HEADER,
    TOKEN_EOF,
    TOKEN_UNKNOWN,

    // Specific
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_DEFAULT,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_RETURN,
    TOKEN_DO,
    TOKEN_GOTO,

    // Data Types
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_VOID,
    TOKEN_STRUCT,
    TOKEN_TYPEDEF,
    TOKEN_ENUM,
    TOKEN_UNION,
    TOKEN_SIZEOF,

    // Symbols for control flow and structure
    TOKEN_OPEN_BRACE,    // {
    TOKEN_CLOSE_BRACE,   // }
    TOKEN_OPEN_PAREN,    // (
    TOKEN_CLOSE_PAREN,   // )
    TOKEN_OPEN_BRACKET,  // [
    TOKEN_CLOSE_BRACKET, // ]
    TOKEN_SEMICOLON,     // ;
    TOKEN_COLON,         // :
    TOKEN_COMMA,         // ,
    TOKEN_ASSIGN,        // =

    // Logical and arithmetic operators

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_INCREMENT, // ++
    TOKEN_DECREMENT, // --
    TOKEN_EQUAL,     // ==
    TOKEN_NOT_EQUAL, // !=
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_LOGICAL_AND,   // &&
    TOKEN_LOGICAL_OR,    // ||
    TOKEN_LOGICAL_NOT,   // !

    // Bitwise operators
    TOKEN_BITWISE_AND,
    TOKEN_BITWISE_OR,
    TOKEN_BITWISE_XOR,
    TOKEN_BITWISE_NOT,
    TOKEN_SHIFT_LEFT,
    TOKEN_SHIFT_RIGHT,


    // extra
    TOKEN_COMMENT_SINGLE_LINE,
    TOKEN_COMMENT_MULTI_LINE,

} TokenType;

// Token structure
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
    int line;
} Token;

// Function declarations
void init_lexer(const char *filename);
Token get_next_token();
void close_lexer();

#endif // LEXER_H
