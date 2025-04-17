
#include "lexer.h"
#include "parser.h"

int main() {
    init_lexer("tests/test.c");

  
    // // parse the program and create the AST
    // ASTNode *root = parse_program();

    // // print the AST for debugging
    // print_ast(root, 0);
    

    Token token;
    do {
        token = get_next_token();
        printf("Token: Type=%d, Value=%s, Line=%d\n", token.type, token.value, token.line);
    } while (token.type != TOKEN_EOF);

    close_lexer();
    return 0;
}


// parser not complete yet will do tomorrow