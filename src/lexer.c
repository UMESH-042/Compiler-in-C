#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

char current_char;
int line = 1;
FILE *source;

static void next_char()
{
    current_char = fgetc(source);
    if (current_char == '\n')
    {
        line++;
    }
}

static Token create_token(TokenType type, const char *value)
{
    Token token;
    token.type = type;
    strncpy(token.value, value, MAX_TOKEN_LENGTH);
    token.line = line;
    return token;
}

static void skip_whitespace()
{
    while (isspace(current_char))
    {
        next_char();
    }
}

void init_lexer(const char *filename)
{
    source = fopen(filename, "r");
    if (!source)
    {
        perror("Error opening file");
        close_lexer();
        exit(EXIT_FAILURE);
    }
    next_char();
}

void close_lexer()
{
    if (source)
    {
        fclose(source);
        source = NULL;
    }
}

const char *keywords[] = {"int", "float", "if", "return", "else", "while", "for", "void", "char", NULL};

Token get_next_token()
{
    skip_whitespace();



    // End of file
    if (current_char == EOF)
    {
        return create_token(TOKEN_EOF, "EOF");
    }


    // Handle single-line (//) and multi-line (/* */) comments
if (current_char == '/') {
    next_char();
    
    // Single-line comment (// ...)
    if (current_char == '/') {  
        while (current_char != '\n' && current_char != EOF) {
            next_char();  
        }
        return get_next_token();  // Skip comment and get the next token
    }

    // Multi-line comment (/* ... */)
    if (current_char == '*') {  
        next_char();
        while (current_char != EOF) {
            if (current_char == '*') {
                next_char();
                if (current_char == '/') {
                    next_char();  // Exit comment
                    return get_next_token();  // Skip comment and get next token
                }
            } else {
                next_char();
            }
        }
    }
}


    // Preprocessor directive
    if (current_char == '#')
    {
        char directive[MAX_TOKEN_LENGTH] = "#";
        next_char();
        while (isalpha(current_char))
        {
            // strncat(directive, &current_char, 1);
            size_t len = strlen(directive);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                directive[len] = current_char;
                directive[len + 1] = '\0';
            }
            next_char();
        }

        return create_token(TOKEN_PREPROCESSOR, directive);
    }

    // Header file <stdio.h>
    if (current_char == '<')
    {
        char header[MAX_TOKEN_LENGTH] = "<";
        next_char();
        while (isalnum(current_char) || current_char == '.')
        {
            // strncat(header, &current_char, 1);
            size_t len = strlen(header);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                header[len] = current_char;
                header[len + 1] = '\0';
            }
            next_char();
        }
        if (current_char == '>')
        {
            // strncat(header, ">", 1);
            size_t len = strlen(header);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                header[len] = '>';
                header[len + 1] = '\0';
            }
            next_char();
            return create_token(TOKEN_HEADER, header);
        }
    }

    // Identifiers (variables, keywords)
    if (isalpha(current_char) || current_char == '_')
    {
        char identifier[MAX_TOKEN_LENGTH] = "";
        while (isalnum(current_char) || current_char == '_')
        {
            // strncat(identifier, &current_char, 1);
            size_t len = strlen(identifier);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                identifier[len] = current_char;
                identifier[len + 1] = '\0';
            }
            next_char();
        }

        for (int i = 0; keywords[i] != NULL; i++)
        {
            if (strcmp(identifier, keywords[i]) == 0)
            {
                return create_token(TOKEN_KEYWORD, identifier);
            }
        }

        return create_token(TOKEN_IDENTIFIER, identifier);
    }

    // Numbers (integer and float)
    if (isdigit(current_char))
    {
        char number[MAX_TOKEN_LENGTH] = "";
        int dot_count = 0;
        while (isdigit(current_char) || current_char == '.' && dot_count == 0)
        {
            if (current_char == '.')
                dot_count++;
            // strncat(number, &current_char, 1);
            size_t len = strlen(number);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                number[len] = current_char;
                number[len + 1] = '\0';
            }
            next_char();
        }
        return create_token(dot_count ? TOKEN_NUMBER : TOKEN_NUMBER, number);
    }

    // String literals "Hello, World!"
    if (current_char == '"')
    {
        char str[MAX_TOKEN_LENGTH] = "\"";
        next_char();
        while (current_char != '"' && current_char != EOF)
        {
            // strncat(str, &current_char, 1);
            size_t len = strlen(str);
            if (len < MAX_TOKEN_LENGTH - 1)
            {
                str[len] = current_char;
                str[len + 1] = '\0';
            }
            next_char();
        }
        // strncat(str, "\"", 1);
        size_t len = strlen(str);
        if (len < MAX_TOKEN_LENGTH - 1)
        {
            str[len] = '"';
            str[len + 1] = '\0';
        }
        next_char();
        return create_token(TOKEN_STRING, str);
    }

    // Operators (==, >=, <=, !=, +, -, *, /, =, etc.)
    if (strchr("=<>!+-*/", current_char))
    {
        char op[MAX_TOKEN_LENGTH] = {current_char, '\0'};
        next_char();
        if ((op[0] == '=' && current_char == '=') ||
            (op[0] == '>' && current_char == '=') ||
            (op[0] == '<' && current_char == '=') ||
            (op[0] == '!' && current_char == '='))
        {
            op[1] = current_char;
            op[2] = '\0';
            next_char();
        }
        return create_token(TOKEN_OPERATOR, op);
    }

    // Separators (parentheses, brackets, semicolons, commas)
    if (strchr("(){}[],;", current_char))
    {
        char sep[2] = {current_char, '\0'};
        next_char();
        return create_token(TOKEN_SEPARATOR, sep);
    }

    // Unknown character
    char unknown[2] = {current_char, '\0'};
    next_char();
    return create_token(TOKEN_UNKNOWN, unknown); // Treat as identifier for now
}

// void tokenize_file(const char *filename)
// {
//     source = fopen(filename, "r");
//     if (!source)
//     {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }

//     next_char();
//     Token token;
//     do
//     {
//         token = get_next_token();
//         printf("Token: Type=%d, Value=%s, Line=%d\n", token.type, token.value, token.line);
//     } while (token.type != TOKEN_EOF);

//     fclose(source);
// }

const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_KEYWORD:
        return "KEYWORD";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_OPERATOR:
        return "OPERATOR";
    case TOKEN_SEPARATOR:
        return "SEPARATOR";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_PREPROCESSOR:
        return "PREPROCESSOR";
    case TOKEN_HEADER:
        return "HEADER";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
    default:
        return "INVALID";
    }
}

void tokenize_file(const char *filename)
{
    init_lexer(filename);
    Token token;
    do
    {
        token = get_next_token();
        printf("Token: Type=%d (%s), Value=\"%s\", Line=%d\n",
               token.type, token_type_to_string(token.type), token.value, token.line);
    } while (token.type != TOKEN_EOF);
    close_lexer();
}
