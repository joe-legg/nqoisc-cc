#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../src/vector.h"

int is_int(char *str)
{
    char *p = NULL;
    strtol(str, &p, 10);
    return !*p;
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

    return token;
}

Vector *lex(FILE *fp)
{
    printf("%s\n", scan_while(fp, (int (*)(char c))&isalnum));
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    FILE *asm_fp = fopen(argv[1], "r");
    if (asm_fp == NULL)
        return 1;

    lex(asm_fp);

    fclose(asm_fp);
}
