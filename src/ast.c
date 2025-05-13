#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode *create_node(ASTNodeType type)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = node->right = NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

void add_child(ASTNode *parent, ASTNode *child)
{
    parent->children = realloc(parent->children, sizeof(ASTNode *) * (parent->child_count + 1));
    parent->children[parent->child_count++] = child;
}

void free_ast(ASTNode *node)
{
    if (!node)
        return;

    for (int i = 0; i < node->child_count; i++)
    {
        free_ast(node->children[i]);
    }
    free(node->children);
    free(node);
}

void print_ast(ASTNode *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");

    switch (node->type)
    {
    case AST_PROGRAM:
        printf("Program\n");
        break;
    case AST_VAR_DECL:
        printf("VarDecl: %s\n", node->token.value);
        break;
    case AST_ASSIGNMENT:
        printf("Assign to: %s\n", node->token.value);
        break;
    case AST_BINARY_EXPR:
        printf("BinaryExpr: %s\n", node->token.value);
        break;
    case AST_NUMBER_LITERAL:
        printf("Number: %s\n", node->token.value);
        break;
    case AST_IDENTIFIER:
        printf("Identifier: %s\n", node->token.value);
        break;
    default:
        printf("Unknown Node\n");
    }

    if (node->left)
        print_ast(node->left, indent + 1);
    if (node->right)
        print_ast(node->right, indent + 1);
    for (int i = 0; i < node->child_count; i++)
    {
        print_ast(node->children[i], indent + 1);
    }
}
