#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum
{
    AST_PROGRAM,
    AST_FUNCTION,
    AST_FUNCTION_CALL,
    AST_DECLARATION,
    AST_EXPRESSION,
    AST_IF_STATEMENT,
    AST_WHILE_LOOP,
    AST_FOR_LOOP,
    AST_SWITCH_STATEMENT,
    AST_CASE_STATEMENT,
    AST_BLOCK,
    AST_RETURN,
    AST_ASSIGNMENT,
    AST_BREAK,
    AST_CONTINUE
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    Token token;
    struct ASTNode *left;  // Left child
    struct ASTNode *right; // Right child
    struct ASTNode *next;  // Next sibling
} ASTNode;

ASTNode *parse_program();
ASTNode *parse_statement();
ASTNode *parse_if_statement();
ASTNode *parse_while_loop();
ASTNode *parse_for_loop();
ASTNode *parse_switch_statement();
ASTNode *parse_block();
void print_ast(ASTNode *root, int level);
void free_ast(ASTNode *node);

#endif
