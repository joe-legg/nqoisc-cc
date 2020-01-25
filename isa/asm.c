#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../src/vector.h"

// Parser

// Instruction op-codes
enum {
    INSTR_RIGHT = 0x0,
    INSTR_ADD   = 0x1,
    INSTR_BNZ   = 0x2,
};

static void gen_instr(FILE *outfile, char opcode, uint32_t imm)
{
    uint32_t instr = (opcode << 30) | (imm & 0x3FFFFFFF);
    instr = __bswap_constant_32(instr); // Little-endian to big-endian
    fwrite(&instr, 4, 1, outfile);
}

void parse(Vector *tokens, FILE *outfile)
{
    enum {
        PARSE_NONE,
        PARSE_RIGHT,
        PARSE_LEFT,
        PARSE_ADD,
        PARSE_SUB,
        PARSE_BNZ,
    } parse_state = PARSE_NONE;

    for (int i = 0; i < tokens->length; i++) {
        char *curtok = tokens->items[i];

        if (strcmp(curtok, "right") == 0) {
            parse_state = PARSE_RIGHT;
        } else if (strcmp(curtok, "left") == 0) {
            parse_state = PARSE_LEFT;
        } else if (strcmp(curtok, "add") == 0) {
            parse_state = PARSE_ADD;
        } else if (strcmp(curtok, "sub") == 0) {
            parse_state = PARSE_SUB;
        } else if (strcmp(curtok, "bnz") == 0) {
            parse_state = PARSE_BNZ;
        } else if (isdigit(curtok[0])) { // Instruction parameter
            switch (parse_state) {
            case PARSE_RIGHT:
                gen_instr(outfile, INSTR_RIGHT, strtol(curtok, NULL, 10));
                break;
            case PARSE_LEFT:
                gen_instr(outfile, INSTR_RIGHT, -strtol(curtok, NULL, 10));
                break;
            case PARSE_ADD:
                gen_instr(outfile, INSTR_ADD, strtol(curtok, NULL, 10));
                break;
            case PARSE_SUB:
                gen_instr(outfile, INSTR_ADD, -strtol(curtok, NULL, 10));
                break;
            case PARSE_BNZ:
                gen_instr(outfile, INSTR_BNZ, strtol(curtok, NULL, 10));
                break;
            default:
                printf("Error: unexpected value \"%s\".", curtok);
                exit(1);
            }
            parse_state = PARSE_NONE;
        } else if (curtok[strlen(curtok) - 1] == ':') { // Label definition
            // TODO
        } else if (isalpha(curtok[0])) { // Label reference
            if (parse_state == PARSE_BNZ) {
                // TODO
            } else {
                printf("Error: unexpected value \"%s\".", curtok);
                exit(1);
            }
        } else {
        }
    }
}

// Lexer

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
    int instr_count = 0;

    if (isalpha(fpeek(fp))) { // Labels and instructions
        token = scan_while(fp, (int (*)(char c))&isalnum);


        if (fpeek(fp) == ':') { // Check for label
            token = append_char(token, ':');
            fgetc(fp);
            
        // Calculate label location
        } else if (strcmp(token, "right") == 0 || strcmp(token, "left") == 0 ||
                   strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 ||
                   strcmp(token, "bnz") == 0) {
            instr_count++;
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

// Main

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    FILE *asm_fp = fopen(argv[1], "r");
    if (asm_fp == NULL)
        return 1;

    Vector *tokens = lex(asm_fp);

    fclose(asm_fp);

    FILE *outfile = fopen("out.bin", "w");
    parse(tokens, outfile);
    fclose(outfile);

    for (int i = 0; i < tokens->length; i++)
        free(tokens->items[i]);
    vector_free(tokens);
}
