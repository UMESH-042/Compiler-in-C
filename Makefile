CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/lexer.c src/parser.c src/ast.c src/semantic.c src/codegen.c src/symbol_table.c
OBJ = $(SRC:.c=.o)
EXEC = ccompiler

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
