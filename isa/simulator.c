#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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

void print_help()
{
    printf("ISA Simulator.\n\n"
           "-h              Print this help and exit.\n"
           "-m <size>       Set the amount of memory in bytes. (The default is 65536)\n"
           "-b <filename>   Specify an input file.\n");
}

int main(int argc, char *argv[])
{
    char *input_program = NULL;
    for (int option; (option = getopt(argc, argv, "m:b:h")) != -1;) {
        switch (option) {
        // Memory size
        case 'm':
            for (int i = 0; optarg[i] != '\0'; i++) {
                if (!(optarg[i] >= '0' && optarg[i] <= '9')) {
                    printf("Invalid memory size.");
                    return 1;
                }
            }
            mem_size = atoi(optarg);
            break;
        // Program to be simulated
        case 'b':
            input_program = optarg;
            break;
        // Help
        case 'h':
            print_help();
            return 0;
            break;
        case '?':
            printf("Unkown option: %c\n", optopt);
            print_help();
            return 1;
            break;
        }
    }

    if (input_program == NULL) {
        printf("No program specified.\n");
        print_help();
        return 1;
    }

    // If memory size is not specified. Set the memory size.
    if (mem_size == 0)
        mem_size = 2 ^ 16;

    load_program(input_program);
    run_processor();

    return 0;
}
