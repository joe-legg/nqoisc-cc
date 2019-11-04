#!/bin/bash
flex src/lexer.l
lemon src/parser.lemon
gcc -g -Wall -c lex.yy.c
gcc -g -Wall -c src/vector.c
gcc -g -Wall -c src/hash_table.c
gcc -g -Wall -c src/error.c
gcc -g -Wall -c src/ast.c
gcc -g -Wall -c src/parser.c
gcc -g -Wall -c src/main.c
gcc -g -Wall -c src/ir.c
gcc -g -Wall -o jcc lex.yy.o parser.o error.o main.o ir.o vector.o ast.o
