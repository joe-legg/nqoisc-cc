#!/bin/bash
flex src/lexer.l
lemon src/parser.lemon
gcc -g -c lex.yy.c
gcc -g -c src/vector.c
gcc -g -c src/error.c
gcc -g -c src/ast.c
gcc -g -c src/parser.c
gcc -g -c src/main.c
gcc -g -c src/ir.c
gcc -g -o jcc lex.yy.o parser.o error.o main.o ir.o vector.o ast.o
