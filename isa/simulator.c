#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

uint32_t memory_dump = 0;

uint8_t *memory;
uint32_t mem_size;

uint32_t program_counter;
uint32_t data_ptr;

int single_step_mode = 0;

int sign_extend_imm(int imm)
{
    int mask = 1U << (30 - 1);
    return (imm ^ mask) - mask;
}

// Read 4 bytes from memory
uint32_t read_4_bytes(uint32_t addr)
{
    if (addr + 3 > mem_size) {
        printf("Error: invalid read from address 0x%x. Consider increasing the "
               "memory size.\n", addr);
        exit(0);
    }

    return memory[addr] << 24     |
           memory[addr + 1] << 16 |
           memory[addr + 2] << 8  |
           memory[addr + 3];
}

// Write 4 bytes
void write_4_bytes(uint32_t addr, uint32_t bytes)
{
    if (addr + 3 > mem_size) {
        printf("Error: invalid write to address 0x%x. Consider increasing the "
               "memory size.\n", addr);
        exit(0);
    }

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
            program_counter = (int32_t)program_counter + (int32_t)imm;
        break;
    }
}

void dump_status(uint32_t instr)
{
    printf("Status\n------\n");

    // Instruction
    uint8_t opcode = instr >> 30;
    uint32_t imm = sign_extend_imm(instr & 0x3FFFFFFF);

    switch (opcode) {
    case 0: printf("instruction: right\n"); break;
    case 1: printf("instruction: add\n"); break;
    case 2: printf("instruction: bnz\n"); break;
    }
    printf("immediate: 0x%x\n\n", imm);

    // Program counter
    printf("program counter: %d\n\n", program_counter);
    if (((int32_t)program_counter - 8) >= 0) {
        printf("0x%x\n", read_4_bytes(program_counter - 8));
        printf("0x%x\n", read_4_bytes(program_counter - 4));
    }
    if (((int32_t)program_counter + 8) < mem_size) {
        printf("0x%x <- program counter\n", read_4_bytes(program_counter));
        printf("0x%x\n", read_4_bytes(program_counter + 4));
        printf("0x%x\n\n", read_4_bytes(program_counter + 8));
    }

    // Data pointer
    printf("data pointer: %d\n\n", data_ptr);
    if (((int32_t)data_ptr - 8) >= 0) {
        printf("0x%x\n", read_4_bytes(data_ptr - 8));
        printf("0x%x\n", read_4_bytes(data_ptr - 4));
    }
    printf("0x%x <- data pointer\n", read_4_bytes(data_ptr));
    if (((int32_t)data_ptr + 8) < mem_size) {
        printf("0x%x\n", read_4_bytes(data_ptr + 4));
        printf("0x%x\n\n", read_4_bytes(data_ptr + 8));
    }
}

void run_processor()
{
    while (1) {
        uint32_t cur_instr = fetch();
        execute(cur_instr);

        if (single_step_mode) {
            dump_status(cur_instr);
            printf("Press enter to continue...\n");
            getchar();
        }
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

void handle_ctrl_c()
{
    if (memory_dump > mem_size) {
        printf("Memory dump to large! Memory size = %d.\n", mem_size);
    } else {
        for (int i = 0; i < memory_dump; i++)
            printf("%c", memory[i]);
    }

    dump_status(read_4_bytes(program_counter));

    free(memory);
    exit(1);
}

void print_help()
{
    printf("ISA Simulator.\n\n"
           "Press ctrl+c to stop the simulator.\n\n"
           "-h              Print this help and exit.\n"
           "-m <size>       Set the amount of memory in bytes. (The default is 65536)\n"
           "-d <size>       Dump <size> bytes of memory to stdout.\n"
           "-b <filename>   Specify an input file.\n"
           "-s              Single step mode.\n");
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_ctrl_c);

    char *input_program = NULL;
    for (int option; (option = getopt(argc, argv, "sm:b:d:h")) != -1;) {
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
        // Memory dump
        case 'd':
            memory_dump = atoi(optarg);
            break;
        // Single step
        case 's':
            single_step_mode = 1;
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

    if (mem_size == 0)
        mem_size = 2 ^ 16;

    load_program(input_program);
    run_processor();

    return 0;
}
