CFLAGS = -g -Wall

jcc: parser.o lex.yy.o main.o error.o
	$(CC) $(CFLAGS) -o jcc main.o lex.yy.o parser.o error.o

main.o: src/main.c src/jcc.h
	$(CC) $(CFLAGS) -c src/main.c

parser.o: src/parser.lemon src/jcc.h
	lemon src/parser.lemon
	$(CC) $(CFLAGS) -c src/parser.c

lex.yy.o: src/lexer.l src/parser.h src/parser.h
	flex src/lexer.l
	$(CC) $(CFLAGS) -c lex.yy.c

error.o: src/error.c src/jcc.h
	$(CC) $(CFLAGS) -c src/error.c

clean:
	rm jcc
	rm src/parser.out
	rm src/parser.h
	rm src/parser.c
	rm lex.yy.c
	rm *.o
