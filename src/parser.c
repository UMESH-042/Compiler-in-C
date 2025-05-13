#include "parser.h"
#include "lexer.h"

Token current_token;

static void advance()
{
    current_token = get_next_token();
}

static int match(TokenType type)
{
    if (current_token.type == type)
    {
        advance();
        return 1;
    }
    return 0;
}

static ASTNode *parse_primary()
{
    if (current_token.type == TOKEN_NUMBER)
    {
        ASTNode *node = create_node(AST_NUMBER_LITERAL);
        node->token = current_token;
        advance();
        return node;
    }

    if (current_token.type == TOKEN_IDENTIFIER)
    {
        ASTNode *node = create_node(AST_IDENTIFIER);
        node->token = current_token;
        advance();
        return node;
    }

    return NULL;
}

static ASTNode *parse_expression()
{
    ASTNode *left = parse_primary();

    if (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS || current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE)
    {
        ASTNode *node = create_node(AST_BINARY_EXPR);
        node->token = current_token;
        advance();
        node->left = left;
        node->right = parse_expression();
        return node;
    }

    return left;
}

static ASTNode *parse_statement()
{
    if (current_token.type == TOKEN_INT)
    {
        advance();
        if (current_token.type == TOKEN_IDENTIFIER)
        {
            ASTNode *decl = create_node(AST_VAR_DECL);
            decl->token = current_token;
            advance();
            match(TOKEN_SEMICOLON);
            return decl;
        }
    }

    if (current_token.type == TOKEN_IDENTIFIER)
    {
        ASTNode *assign = create_node(AST_ASSIGNMENT);
        assign->token = current_token;
        advance();
        match(TOKEN_ASSIGN);
        assign->right = parse_expression();
        match(TOKEN_SEMICOLON);
        return assign;
    }

    return NULL;
}

ASTNode *parse_program()
{
    advance();
    ASTNode *program = create_node(AST_PROGRAM);
    while (current_token.type != TOKEN_EOF)
    {
        ASTNode *stmt = parse_statement();
        if (stmt)
            add_child(program, stmt);
        else
            break;
    }

    return program;
}
