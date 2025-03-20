#include <stdio.h>
#include "../src/lexer.h"

#define MAX 100

int main() {
    char message[] = "Hello, World!";
    int num = 42;
    float pi = 3.14;

    /* this
        is multiline comment
    */
    
    if (num >= MAX) {
        printf("Number is large.\n");
    }
    // this is single line comment

    return 0;
}
