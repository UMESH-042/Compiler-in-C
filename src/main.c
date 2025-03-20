#include "lexer.h"

int main() {
    init_lexer("tests/test.c");

    Token token;
    do {
        token = get_next_token();
        printf("Token: Type=%d, Value=%s, Line=%d\n", token.type, token.value, token.line);
    } while (token.type != TOKEN_EOF);

    close_lexer();
    return 0;
}

