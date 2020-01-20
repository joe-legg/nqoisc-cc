#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../src/vector.h"

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

char *next_tok(FILE *fp)
{
    scan_while(fp, (int (*)(char c))&isspace); // Skip whitespace

    char *token;

    if (isalpha(fpeek(fp))) { // Labels and instructions
        token = scan_while(fp, (int (*)(char c))&isalnum);

        if (fpeek(fp) == ':') { // Check for label
            token = append_char(token, ':');
            fgetc(fp);
        }

    } else if (isdigit(fpeek(fp))) { // Integers
        token = scan_while(fp, (int (*)(char c))&isdigit);

    } else if (fpeek(fp) == EOF) { // EOF
        return NULL;

    } else {
        printf("Error unkown token \"%c\".\n", fpeek(fp));
        exit(1);
    }

    return token;
}

Vector *lex(FILE *fp)
{
    char *token;
    Vector *tok_vec = new_vector();

    while ((token = next_tok(fp)) != NULL)
        vector_append(tok_vec, token);

    return tok_vec;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    FILE *asm_fp = fopen(argv[1], "r");
    if (asm_fp == NULL)
        return 1;

    Vector *tokens = lex(asm_fp);
    for (int i = 0; i < tokens->length; i++)
        printf("%s\n", (char *)tokens->items[i]);

    fclose(asm_fp);
}
