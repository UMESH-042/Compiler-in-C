
#include "lexer.h"
#include "parser.h"

int main()
{
    init_lexer("tests/test.c");

    printf("=== Lexer Output ===\n");
    Token token;
    do
    {
        token = get_next_token();
        printf("Token: Type=%d, Value=%s, Line=%d\n", token.type, token.value, token.line);
    } while (token.type != TOKEN_EOF);

    printf("\n=== Parser & AST Output ===\n");
    init_lexer("tests/test.c");
    ASTNode *root = parse_program();
    print_ast(root, 0);
    free_ast(root);

    close_lexer();
    return 0;
}

// parser not complete yet will do tomorrow