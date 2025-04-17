#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

char current_char;
int line = 1;
FILE *source;

static void next_char() {
    current_char = fgetc(source);
    if (current_char == '\n') {
        line++;
    }
}

static Token create_token(TokenType type, const char *value) {
    Token token;
    token.type = type;
    strncpy(token.value, value, MAX_TOKEN_LENGTH);
    token.line = line;
    return token;
}

static void skip_whitespace() {
    while (isspace(current_char)) {
        next_char();
    }
}

void init_lexer(const char *filename) {
    source = fopen(filename, "r");
    if (!source) {
        perror("Error opening file");
        close_lexer();
        exit(EXIT_FAILURE);
    }
    next_char();
}

void close_lexer() {
    if (source) {
        fclose(source);
        source = NULL;
    }
}

typedef struct {
    const char *keyword;
    TokenType type;
} KeywordMap;

const KeywordMap keywords[] = {
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE},
    {"switch", TOKEN_SWITCH},
    {"case", TOKEN_CASE},
    {"default", TOKEN_DEFAULT},
    {"break", TOKEN_BREAK},
    {"continue", TOKEN_CONTINUE},
    {"return", TOKEN_RETURN},
    {"do", TOKEN_DO},
    {"goto", TOKEN_GOTO},
    {"int", TOKEN_INT},
    {"float", TOKEN_FLOAT},
    {"char", TOKEN_CHAR},
    {"void", TOKEN_VOID},
    {"struct", TOKEN_STRUCT},
    {"typedef", TOKEN_TYPEDEF},
    {"enum", TOKEN_ENUM},
    {"union", TOKEN_UNION},
    {"sizeof", TOKEN_SIZEOF},
    {NULL, TOKEN_UNKNOWN}
};

Token get_next_token() {
    skip_whitespace();

    if (current_char == EOF) {
        return create_token(TOKEN_EOF, "EOF");
    }

    // Handle comments
    if (current_char == '/') {
        next_char();
        if (current_char == '/') {
            while (current_char != '\n' && current_char != EOF) {
                next_char();
            }
            return create_token(TOKEN_COMMENT_SINGLE_LINE, "//");
        } else if (current_char == '*') {
            next_char();
            while (current_char != EOF) {
                if (current_char == '*') {
                    next_char();
                    if (current_char == '/') {
                        next_char();
                        return create_token(TOKEN_COMMENT_MULTI_LINE, "/* */");
                    }
                } else {
                    next_char();
                }
            }
        } else {
            return create_token(TOKEN_DIVIDE, "/");
        }
    }

    // Preprocessor directive
    if (current_char == '#') {
        char directive[MAX_TOKEN_LENGTH] = "#";
        next_char();
        while (isalpha(current_char)) {
            size_t len = strlen(directive);
            if (len < MAX_TOKEN_LENGTH - 1) {
                directive[len] = current_char;
                directive[len + 1] = '\0';
            }
            next_char();
        }
        return create_token(TOKEN_PREPROCESSOR, directive);
    }

    // Header file
    if (current_char == '<') {
        char header[MAX_TOKEN_LENGTH] = "<";
        next_char();
        while (isalnum(current_char) || current_char == '.') {
            size_t len = strlen(header);
            if (len < MAX_TOKEN_LENGTH - 1) {
                header[len] = current_char;
                header[len + 1] = '\0';
            }
            next_char();
        }
        if (current_char == '>') {
            size_t len = strlen(header);
            if (len < MAX_TOKEN_LENGTH - 1) {
                header[len] = '>';
                header[len + 1] = '\0';
            }
            next_char();
            return create_token(TOKEN_HEADER, header);
        }
    }

    // Identifiers or keywords
    if (isalpha(current_char) || current_char == '_') {
        char buffer[MAX_TOKEN_LENGTH] = "";
        while (isalnum(current_char) || current_char == '_') {
            size_t len = strlen(buffer);
            if (len < MAX_TOKEN_LENGTH - 1) {
                buffer[len] = current_char;
                buffer[len + 1] = '\0';
            }
            next_char();
        }
        for (int i = 0; keywords[i].keyword != NULL; i++) {
            if (strcmp(buffer, keywords[i].keyword) == 0) {
                return create_token(keywords[i].type, buffer);
            }
        }
        return create_token(TOKEN_IDENTIFIER, buffer);
    }

    // Numbers (integer or float)
    if (isdigit(current_char)) {
        char number[MAX_TOKEN_LENGTH] = "";
        int dot_count = 0;
        while (isdigit(current_char) || (current_char == '.' && dot_count == 0)) {
            if (current_char == '.') dot_count++;
            size_t len = strlen(number);
            if (len < MAX_TOKEN_LENGTH - 1) {
                number[len] = current_char;
                number[len + 1] = '\0';
            }
            next_char();
        }
        return create_token(TOKEN_NUMBER, number);
    }

    // String literal
    if (current_char == '"') {
        char str[MAX_TOKEN_LENGTH] = "\"";
        next_char();
        while (current_char != '"' && current_char != EOF) {
            size_t len = strlen(str);
            if (len < MAX_TOKEN_LENGTH - 1) {
                str[len] = current_char;
                str[len + 1] = '\0';
            }
            next_char();
        }
        size_t len = strlen(str);
        if (len < MAX_TOKEN_LENGTH - 1) {
            str[len] = '"';
            str[len + 1] = '\0';
        }
        next_char();
        return create_token(TOKEN_STRING, str);
    }

    // Char literal
    if (current_char == '\'') {
        char ch_lit[MAX_TOKEN_LENGTH] = "\'";
        next_char();
        if (current_char != '\'' && current_char != EOF) {
            size_t len = strlen(ch_lit);
            ch_lit[len] = current_char;
            ch_lit[len + 1] = '\0';
            next_char();
        }
        if (current_char == '\'') {
            size_t len = strlen(ch_lit);
            ch_lit[len] = '\'';
            ch_lit[len + 1] = '\0';
            next_char();
        }
        return create_token(TOKEN_CHAR_LITERAL, ch_lit);
    }

    // Symbols and Operators
    char sym = current_char;
    next_char();
    switch (sym) {
        case '+': return current_char == '+' ? (next_char(), create_token(TOKEN_INCREMENT, "++")) : create_token(TOKEN_PLUS, "+");
        case '-': return current_char == '-' ? (next_char(), create_token(TOKEN_DECREMENT, "--")) : create_token(TOKEN_MINUS, "-");
        case '*': return create_token(TOKEN_MULTIPLY, "*");
        case '%': return create_token(TOKEN_MODULO, "%");
        case '=': return current_char == '=' ? (next_char(), create_token(TOKEN_EQUAL, "==")) : create_token(TOKEN_ASSIGN, "=");
        case '!': return current_char == '=' ? (next_char(), create_token(TOKEN_NOT_EQUAL, "!=")) : create_token(TOKEN_LOGICAL_NOT, "!");
        case '<': return current_char == '=' ? (next_char(), create_token(TOKEN_LESS_EQUAL, "<=")) :
                        current_char == '<' ? (next_char(), create_token(TOKEN_SHIFT_LEFT, "<<")) :
                        create_token(TOKEN_LESS_THAN, "<");
        case '>': return current_char == '=' ? (next_char(), create_token(TOKEN_GREATER_EQUAL, ">=")) :
                        current_char == '>' ? (next_char(), create_token(TOKEN_SHIFT_RIGHT, ">>")) :
                        create_token(TOKEN_GREATER_THAN, ">");
        case '&': return current_char == '&' ? (next_char(), create_token(TOKEN_LOGICAL_AND, "&&")) : create_token(TOKEN_BITWISE_AND, "&");
        case '|': return current_char == '|' ? (next_char(), create_token(TOKEN_LOGICAL_OR, "||")) : create_token(TOKEN_BITWISE_OR, "|");
        case '^': return create_token(TOKEN_BITWISE_XOR, "^");
        case '~': return create_token(TOKEN_BITWISE_NOT, "~");
        case '(': return create_token(TOKEN_OPEN_PAREN, "(");
        case ')': return create_token(TOKEN_CLOSE_PAREN, ")");
        case '{': return create_token(TOKEN_OPEN_BRACE, "{");
        case '}': return create_token(TOKEN_CLOSE_BRACE, "}");
        case '[': return create_token(TOKEN_OPEN_BRACKET, "[");
        case ']': return create_token(TOKEN_CLOSE_BRACKET, "]");
        case ';': return create_token(TOKEN_SEMICOLON, ";");
        case ',': return create_token(TOKEN_COMMA, ",");
        case ':': return create_token(TOKEN_COLON, ":");
    }

    // Unknown character
    char unknown[2] = {sym, '\0'};
    return create_token(TOKEN_UNKNOWN, unknown);
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_SEPARATOR: return "SEPARATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_HEADER: return "HEADER";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_SWITCH: return "SWITCH";
        case TOKEN_CASE: return "CASE";
        case TOKEN_DEFAULT: return "DEFAULT";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_DO: return "DO";
        case TOKEN_GOTO: return "GOTO";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_VOID: return "VOID";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_TYPEDEF: return "TYPEDEF";
        case TOKEN_ENUM: return "ENUM";
        case TOKEN_UNION: return "UNION";
        case TOKEN_SIZEOF: return "SIZEOF";
        case TOKEN_OPEN_BRACE: return "OPEN_BRACE";
        case TOKEN_CLOSE_BRACE: return "CLOSE_BRACE";
        case TOKEN_OPEN_PAREN: return "OPEN_PAREN";
        case TOKEN_CLOSE_PAREN: return "CLOSE_PAREN";
        case TOKEN_OPEN_BRACKET: return "OPEN_BRACKET";
        case TOKEN_CLOSE_BRACKET: return "CLOSE_BRACKET";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COLON: return "COLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";
        case TOKEN_INCREMENT: return "INCREMENT";
        case TOKEN_DECREMENT: return "DECREMENT";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS_THAN: return "LESS_THAN";
        case TOKEN_GREATER_THAN: return "GREATER_THAN";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_LOGICAL_AND: return "LOGICAL_AND";
        case TOKEN_LOGICAL_OR: return "LOGICAL_OR";
        case TOKEN_LOGICAL_NOT: return "LOGICAL_NOT";
        case TOKEN_BITWISE_AND: return "BITWISE_AND";
        case TOKEN_BITWISE_OR: return "BITWISE_OR";
        case TOKEN_BITWISE_XOR: return "BITWISE_XOR";
        case TOKEN_BITWISE_NOT: return "BITWISE_NOT";
        case TOKEN_SHIFT_LEFT: return "SHIFT_LEFT";
        case TOKEN_SHIFT_RIGHT: return "SHIFT_RIGHT";
        case TOKEN_COMMENT_SINGLE_LINE: return "COMMENT_SINGLE_LINE";
        case TOKEN_COMMENT_MULTI_LINE: return "COMMENT_MULTI_LINE";
        default: return "INVALID";
    }
}

void tokenize_file(const char *filename) {
    init_lexer(filename);
    Token token;
    do {
        token = get_next_token();
        printf("Token: Type=%s, Value=\"%s\", Line=%d\n",
               token_type_to_string(token.type), token.value, token.line);
    } while (token.type != TOKEN_EOF);
    close_lexer();
}
