#!/bin/bash
flex src/lexer.l
lemon src/parser.lemon
gcc -c lex.yy.c
gcc -c src/vector.c
gcc -c src/error.c
gcc -c src/ast.c
gcc -c src/parser.c
gcc -c src/main.c
gcc -o jcc lex.yy.o parser.o error.o main.o vector.o ast.o
