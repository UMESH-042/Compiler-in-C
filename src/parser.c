#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Create AST Node
ASTNode *create_ast_node(ASTNodeType type, Token token)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = type;
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

// Free the AST to prevent memory leaks
void free_ast(ASTNode *node)
{
    if (!node)
        return;
    
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->next);
    
    free(node);
}

// Indentation helper
void print_indent(int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("    ");  // Four spaces per level
    }
}

// Recursively print AST
void print_ast(ASTNode *root, int level)
{
    if (!root)
        return;

    print_indent(level);
    printf("Node: %s, Token: %s\n",
           (root->type == AST_PROGRAM) ? "PROGRAM" :
           (root->type == AST_FUNCTION) ? "FUNCTION" :
           (root->type == AST_FUNCTION_CALL) ? "FUNCTION_CALL" :
           (root->type == AST_DECLARATION) ? "DECLARATION" :
           (root->type == AST_EXPRESSION) ? "EXPRESSION" :
           (root->type == AST_IF_STATEMENT) ? "IF_STATEMENT" :
           (root->type == AST_WHILE_LOOP) ? "WHILE_LOOP" :
           (root->type == AST_FOR_LOOP) ? "FOR_LOOP" :
           (root->type == AST_SWITCH_STATEMENT) ? "SWITCH_STATEMENT" :
           (root->type == AST_BLOCK) ? "BLOCK" :
           (root->type == AST_RETURN) ? "RETURN" :
           (root->type == AST_ASSIGNMENT) ? "ASSIGNMENT" : "UNKNOWN",
           root->token.value);

    if (root->left)
    {
        print_indent(level + 1);
        printf("Left:\n");
        print_ast(root->left, level + 1);
    }

    if (root->right)
    {
        print_indent(level + 1);
        printf("Right:\n");
        print_ast(root->right, level + 1);
    }

    if (root->next)
    {
        print_indent(level);
        printf("Next:\n");
        print_ast(root->next, level);
    }
}

// Parse a block of statements { ... }
ASTNode *parse_block()
{
    Token startToken = get_next_token(); // Consume '{'
    
    ASTNode *blockNode = create_ast_node(AST_BLOCK, startToken);
    ASTNode *current = NULL;

    while (1)
    {
        Token token = get_next_token();
        if (strcmp(token.value, "}") == 0) break;

        ASTNode *stmt = parse_statement();
        if (!stmt) continue;

        if (!blockNode->left)
        {
            blockNode->left = stmt;
            current = stmt;
        }
        else
        {
            current->next = stmt;
            current = stmt;
        }
    }

    return blockNode;
}

// Parse an if-statement: if (condition) { block } [else { block }]
ASTNode *parse_if_statement()
{
    Token ifToken = get_next_token();    // Consume 'if'
    Token condition = get_next_token();  // Get condition
    ASTNode *ifNode = create_ast_node(AST_IF_STATEMENT, ifToken);
    ifNode->left = create_ast_node(AST_EXPRESSION, condition);  // Condition

    ifNode->right = parse_block();       // Parse block

    // Check for else part
    Token nextToken = get_next_token();
    if (strcmp(nextToken.value, "else") == 0)
    {
        get_next_token();  // Consume 'else'
        ifNode->next = parse_block();
    }

    return ifNode;
}

// Parse while-loop: while (condition) { block }
ASTNode *parse_while_loop()
{
    Token whileToken = get_next_token(); // Consume 'while'
    Token condition = get_next_token();  // Condition
    ASTNode *whileNode = create_ast_node(AST_WHILE_LOOP, whileToken);

    whileNode->left = create_ast_node(AST_EXPRESSION, condition); // Condition
    whileNode->right = parse_block(); // Block

    return whileNode;
}

// Parse for-loop: for (init; condition; increment) { block }
ASTNode *parse_for_loop()
{
    Token forToken = get_next_token();     // 'for'
    Token init = get_next_token();         // Initialization
    Token condition = get_next_token();    // Condition
    Token increment = get_next_token();    // Increment

    ASTNode *forNode = create_ast_node(AST_FOR_LOOP, forToken);

    ASTNode *initNode = create_ast_node(AST_EXPRESSION, init);
    ASTNode *conditionNode = create_ast_node(AST_EXPRESSION, condition);
    ASTNode *incrementNode = create_ast_node(AST_EXPRESSION, increment);

    initNode->next = conditionNode;       // Link init → condition → increment
    conditionNode->next = incrementNode;

    forNode->left = initNode;             // Attach to for-loop
    forNode->right = parse_block();

    return forNode;
}

// Parse switch-statement: switch (expression) { case ... }
ASTNode *parse_switch_statement()
{
    Token switchToken = get_next_token(); // 'switch'
    Token expr = get_next_token();        // Expression

    ASTNode *switchNode = create_ast_node(AST_SWITCH_STATEMENT, switchToken);
    switchNode->left = create_ast_node(AST_EXPRESSION, expr);

    switchNode->right = parse_block();    // Parse block

    return switchNode;
}

// Parse program
ASTNode *parse_program()
{
    ASTNode *root = NULL;
    ASTNode *current = NULL;

    while (1)
    {
        Token token = get_next_token();
        if (token.type == TOKEN_EOF)
            break;

        ASTNode *stmt = parse_statement();

        if (!stmt) continue;

        if (!root)
        {
            root = stmt;
            current = stmt;
        }
        else
        {
            current->next = stmt;
            current = stmt;
        }
    }

    return root;
}

// Parse statement
ASTNode *parse_statement()
{
    Token token = get_next_token();

    if (strcmp(token.value, "if") == 0) return parse_if_statement();
    if (strcmp(token.value, "while") == 0) return parse_while_loop();
    if (strcmp(token.value, "for") == 0) return parse_for_loop();
    if (strcmp(token.value, "switch") == 0) return parse_switch_statement();

    return NULL;
}
