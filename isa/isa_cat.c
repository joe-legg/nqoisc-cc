/*
A simple program that reads a binary file and prints a formatted english
version of it.
*/

#include <stdio.h>
#include <stdint.h>

uint32_t to_little_endian(uint32_t word)
{
    return (((word >> 0) & 0xff) << 24) |
           (((word >> 8) & 0xff) << 16) |
           (((word >> 16) & 0xff) << 8) |
           (((word >> 24) & 0xff) << 0);
}

// Sign extend 30-bit value
int extend_imm(int imm)
{
    int mask = 1U << (30 - 1);
    return (imm ^ mask) - mask;
}

void cat_file(FILE *fp)
{
    uint32_t instr;
    while (1) {
        // Read the instruction
        fread(&instr, 4, 1, fp);
        if (feof(fp) || ferror(fp)) break;
        instr = to_little_endian(instr);

        switch (instr >> 30) {
        case 0:  printf("right 0x%x\n", extend_imm(instr & 0x3FFFFFFF)); break;
        case 1:  printf("add   0x%x\n", extend_imm(instr & 0x3FFFFFFF)); break;
        case 2:  printf("bnz   0x%x\n", extend_imm(instr & 0x3FFFFFFF)); break;
        default: printf("nop\n"); break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cat_file(stdin);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "rb");
            if (fp == NULL) {
                printf("Error opening file \"%s\".\n", argv[i]);
                return 1;
            }
            cat_file(fp);
            fclose(fp);
        }
    }
}
