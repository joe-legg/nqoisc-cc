#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../src/vector.h"

typedef struct Token {
    enum {
        TOK_RIGHT,
        TOK_LEFT,
        TOK_ADD,
        TOK_SUB,
        TOK_BNZ,
        TOK_INTEGER,
        TOK_LABEL_REF,
    } type;

    char *string;
} Token;

typedef struct Label {
    int location;
    char *name;
} Label;

// Globals

Vector *tokens;
Vector *labels; // Symbol

int fpeek(FILE *fp)
{
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}

char *append_char(char *str, char c)
{
    int len = strlen(str);
    str = realloc(str, sizeof(char) * (len + 2));
    str[len] = c;
    str[len + 1] = '\0';
    return str;
}

char *scan_while(FILE *fp, int (*condition)(char c))
{
    char c;
    char *token = malloc(sizeof(char));
    token[0] = '\0';

    while ((c = fgetc(fp)) != EOF && condition(c))
        token = append_char(token, c);
    ungetc(c, fp);

    return token;
}

void lex(FILE *src)
{
    tokens = new_vector();
    labels = new_vector();

    int instr_index = 0;

    while (1) {
        Token *tok = malloc(sizeof(Token));
        tok->string = malloc(sizeof(char));
        tok->string[0] = '\0';

        scan_while(src, (int (*)(char))&isspace);

        if (fpeek(src) == EOF)
            break;

        // Instruction or label
        if (isalpha(fpeek(src))) {
            tok->string = scan_while(src, (int (*)(char))&isalnum);

            if (fpeek(src) == ':') { // Label definition
                fgetc(src); // Advance the file stream

                // Create label
                Label *l = malloc(sizeof(Label));
                l->name = tok->string;
                l->location = instr_index;

                vector_append(labels, l);

                // Delete the token as it is not needed
                free(tok->string);
                free(tok);
                continue;
            } else {
                // Keywords
                if (strcmp(tok->string, "right") == 0) {
                    instr_index += 4;
                    tok->type = TOK_RIGHT;
                } else if (strcmp(tok->string, "left") == 0) {
                    instr_index += 4;
                    tok->type = TOK_LEFT;
                } else if (strcmp(tok->string, "add") == 0) {
                    instr_index += 4;
                    tok->type = TOK_ADD;
                } else if (strcmp(tok->string, "sub") == 0) {
                    instr_index += 4;
                    tok->type = TOK_SUB;
                } else if (strcmp(tok->string, "bnz") == 0) {
                    instr_index += 4;
                    tok->type = TOK_BNZ;
                // Label reference
                } else {
                    tok->type = TOK_LABEL_REF;
                }
            }
        // Integers
        } else if (isdigit(fpeek(src))) {
            tok->string = scan_while(src, (int (*)(char))&isdigit);
            tok->type = TOK_INTEGER;
        // Error
        } else {
            printf("error: unexpected character \"%c\".\n", fpeek(src));
            exit(1);
        }

        vector_append(tokens, tok);
    }
}

// Main

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    FILE *source = fopen(argv[1], "r");
    lex(source);
    fclose(source);

    for (int i = 0; i < tokens->length; i++) {
        Token *curtok = (Token *)tokens->items[i];
        printf("%s\n", curtok->string);
        free(curtok->string);
        free(curtok);
    }
    vector_free(tokens);
}
