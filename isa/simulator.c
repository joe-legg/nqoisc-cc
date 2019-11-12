#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t *memory;
uint32_t mem_size;

uint32_t program_counter;
uint32_t data_ptr;

int sign_extend_imm(int imm)
{
    int mask = 1U << (30 - 1);
    return (imm ^ mask) - mask;
}

// Read 4 bytes from memory
uint32_t read_4_bytes(uint32_t addr)
{
    return memory[addr] << 24     |
           memory[addr + 1] << 16 |
           memory[addr + 2] << 8  |
           memory[addr + 3];
}

// Write 4 bytes
void write_4_bytes(uint32_t addr, uint32_t bytes)
{
    memory[addr] = bytes >> 24;
    memory[addr + 1] = bytes >> 16;
    memory[addr + 2] = bytes >> 8;
    memory[addr + 3] = bytes;
}

uint32_t fetch()
{
    // Load 4 bytes from memory
    uint32_t instr = read_4_bytes(program_counter);

    if (program_counter + 1 > mem_size) program_counter = 0;
    else program_counter += 4;

    return instr;
}

void execute(uint32_t instr)
{
    uint8_t opcode = instr >> 30;
    uint32_t imm = sign_extend_imm(instr & 0x3FFFFFFF);

    switch (opcode) {
    case 0: // right
        data_ptr += imm;
        break;
    case 1: // add
        write_4_bytes(data_ptr, read_4_bytes(data_ptr) + imm);
        break;
    case 2: // bnz
        if (read_4_bytes(data_ptr) != 0)
            program_counter += imm;
        break;
    }
}

void run_processor()
{
    while (1) {
        execute(fetch());
    }
}

void load_program(const char *filename)
{
    FILE *program = fopen(filename, "rb");

    if (program == NULL) {
        printf("Failed to open the file \"%s\".\n", filename);
        exit(1);
    }

    memory = malloc(sizeof(uint8_t) * mem_size);
    fread(memory, 1, mem_size, program);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("To many or not enough arguments.\n");
        return 1;
    }

    mem_size = 2 ^ 16;

    load_program(argv[1]);
    run_processor();

    return 0;
}
