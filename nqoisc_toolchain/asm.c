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
        TOK_EOF,
    } type;

    char *string;
} Token;

typedef struct Label {
    int location;
    char *name;
} Label;

struct {
    Vector *tokens;
    Vector *labels; // Symbol table
} lexer_ctx;

struct {
    int nxt_tok_index;
    Token *curtok;
    Token *lasttok;
} parser_ctx;

// Code gen

// Instruction opcodes
enum {
    INSTR_RIGHT = 0x0,
    INSTR_ADD   = 0x1,
    INSTR_BNZ   = 0x2,
    INSTR_NOP   = 0x3
};

void gen_instr(FILE *outfile, char opcode, uint32_t imm)
{
    uint32_t instr = (opcode << 30) | (imm & 0x3FFFFFFF);
    instr = __bswap_constant_32(instr); // Little-endian to big-endian
    fwrite(&instr, 4, 1, outfile);
}

// Parser

void next_tok()
{
    parser_ctx.lasttok = parser_ctx.curtok;
    parser_ctx.curtok = lexer_ctx.tokens->items[parser_ctx.nxt_tok_index];
    parser_ctx.nxt_tok_index++;
}

int accept_tok(int tok_type)
{
    if (parser_ctx.curtok->type == tok_type) {
        next_tok();
        return 1;
    }
    return 0;
}

void expect_tok(int tok_type)
{
    if (!accept_tok(tok_type)) {
        printf("error: unexpected token \"%s\".\n", parser_ctx.curtok->string);
        exit(1);
    }
}

void parse(FILE *outfile)
{
    int instr_addr = 0;
    parser_ctx.nxt_tok_index = 0;
    next_tok();

    while (parser_ctx.curtok->type != TOK_EOF) {
        // Right instruction
        if (accept_tok(TOK_RIGHT)) {
            expect_tok(TOK_INTEGER);

            gen_instr(outfile, INSTR_RIGHT,
                      strtol(parser_ctx.lasttok->string, NULL, 10));
            instr_addr += 4;
        // Left instruction
        } else if (accept_tok(TOK_LEFT)) {
            expect_tok(TOK_INTEGER);

            gen_instr(outfile, INSTR_RIGHT,
                      -strtol(parser_ctx.lasttok->string, NULL, 10));
            instr_addr += 4;
        // Add instruction
        } else if (accept_tok(TOK_ADD)) {
            expect_tok(TOK_INTEGER);

            gen_instr(outfile, INSTR_ADD,
                      strtol(parser_ctx.lasttok->string, NULL, 10));
            instr_addr += 4;
        // Subtract instruction
        } else if (accept_tok(TOK_SUB)) {
            expect_tok(TOK_INTEGER);

            gen_instr(outfile, INSTR_ADD,
                      -strtol(parser_ctx.lasttok->string, NULL, 10));
            instr_addr += 4;
        // Branch if not zero instruction
        } else if (accept_tok(TOK_BNZ)) {
            expect_tok(TOK_LABEL_REF);

            char *label_name = parser_ctx.lasttok->string;
            Label *label = NULL;

            // Search the label symbol table
            for (int i = 0; i < lexer_ctx.labels->length; i++) {
                Label *curlabel = lexer_ctx.labels->items[i];

                if (strcmp(curlabel->name, label_name) == 0)
                    label = curlabel;
            }

            if (label == NULL) {
                printf("error: undefined label \"%s\".\n", label_name);
                exit(1);
            }

            // Generate the instruction
            instr_addr += 4;
            gen_instr(outfile, INSTR_BNZ,
                      instr_addr < label->location ?
                          label->location - instr_addr :       // Bnz positive
                          (label->location - instr_addr)); // Bnz negative
        } else {
            printf("error: unexpected token \"%s\".\n",
                   parser_ctx.curtok->string);
            exit(1);
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

int is_not_newline(char c)      { return c != '\n'; } // Used for comments
int is_valid_ident_char(char c) { return isalnum(c) || c == '_'; } // Identifier

// Basic lexer function
void lex(FILE *src)
{
    lexer_ctx.tokens = new_vector();
    lexer_ctx.labels = new_vector();

    int instr_addr = 0;

    while (1) {
        Token *tok = malloc(sizeof(Token));
        tok->string = malloc(sizeof(char));
        tok->string[0] = '\0';

        scan_while(src, (int (*)(char))&isspace);

        if (fpeek(src) == EOF)
            break;

        // Instruction or label
        if (isalpha(fpeek(src))) {
            tok->string = scan_while(src, (int (*)(char))&is_valid_ident_char);

            // Label definition
            // Note: label definitions are not appended to lexer_ctx.tokens,
            //       instead they are appended to lexer_ctx.labels.
            if (fpeek(src) == ':') {
                fgetc(src); // Advance the file stream

                // Create label
                Label *l = malloc(sizeof(Label));
                l->name = strdup(tok->string);
                l->location = instr_addr;

                vector_append(lexer_ctx.labels, l);

                // Delete the token as it is not needed
                free(tok->string);
                free(tok);
                continue;
            } else {
                // Keywords
                if (strcmp(tok->string, "right") == 0) {
                    instr_addr += 4;
                    tok->type = TOK_RIGHT;
                } else if (strcmp(tok->string, "left") == 0) {
                    instr_addr += 4;
                    tok->type = TOK_LEFT;
                } else if (strcmp(tok->string, "add") == 0) {
                    instr_addr += 4;
                    tok->type = TOK_ADD;
                } else if (strcmp(tok->string, "sub") == 0) {
                    instr_addr += 4;
                    tok->type = TOK_SUB;
                } else if (strcmp(tok->string, "bnz") == 0) {
                    instr_addr += 4;
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
        // Comments
        } else if (fpeek(src) == ';') {
            tok->string = scan_while(src, (int (*)(char))&is_not_newline);

            // Delete the token as it is not needed
            free(tok->string);
            free(tok);
            continue;
        // Error
        } else {
            printf("error: unexpected character \"%c\".\n", fpeek(src));
            exit(1);
        }

        vector_append(lexer_ctx.tokens, tok);
    }

    Token *eof = malloc(sizeof(Token));
    eof->type = TOK_EOF;
    vector_append(lexer_ctx.tokens, eof);
}

// Main

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    // TODO: command line argument parsing.
    // TODO: free allocated memory
}
