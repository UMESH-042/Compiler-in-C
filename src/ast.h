#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum
{
    AST_PROGRAM,
    AST_VAR_DECL,
    AST_ASSIGNMENT,
    AST_BINARY_EXPR,
    AST_NUMBER_LITERAL,
    AST_IDENTIFIER,
    AST_EMPTY
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    Token token; // for leaf nodes like literals and identifiers
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode **children;
    int child_count;
} ASTNode;

ASTNode *create_node(ASTNodeType type);
void add_child(ASTNode *parent, ASTNode *child);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int indent);

#endif
