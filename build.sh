#!/bin/bash
flex src/lexer.l
lemon src/parser.lemon
gcc -DDEBUG -g -Wall -c lex.yy.c
gcc -DDEBUG -g -Wall -c src/vector.c
gcc -DDEBUG -g -Wall -c src/hash_table.c
gcc -DDEBUG -g -Wall -c src/malloc_or_die.c
gcc -DDEBUG -g -Wall -c src/error.c
gcc -DDEBUG -g -Wall -c src/ast.c
gcc -DDEBUG -g -Wall -c src/parser.c
gcc -DDEBUG -g -Wall -c src/main.c
gcc -DDEBUG -g -Wall -c src/code_gen.c
gcc -DDEBUG -g -Wall -o nqoisc-cc lex.yy.o parser.o error.o main.o vector.o ast.o malloc_or_die.o code_gen.o
