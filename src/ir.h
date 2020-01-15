#ifndef IR_H
#define IR_H

#include <stdint.h>

// IR Value
typedef struct IrValue {
    enum {
        VALUE_VARIABLE,
        VALUE_INTEGER,
        VALUE_FLOAT,
        VALUE_STRING
    } type;

    union {
        int variable;
        long long integer;
        double floating_point;
        char *string;
    };
} IrValue;

// IR Instruction
typedef struct IrInstr {
    enum {
        // Variables
        IR_INIT,
        IR_NEW_FRAME,
        IR_POP_FRAME,

        // Branching
        IR_RET,
        IR_JAL,
        IR_JNZ,

        // Operations
        IR_ADD,
        IR_SUB,
        IR_MULT,
        IR_DIV
    } type;

    struct IrInstr *next;
    struct IrInstr *branch; // Used in jump instructions

    IrValue p0, p1; // Parameters

    int print_label; // Used to tell ir_print that a label exists here
} IrInstr;

IrInstr *new_ir_instr(int type, IrValue p0, IrValue p1,
                      IrInstr *next, IrInstr *branch);
void free_ir_instr(IrInstr *ir);

void ir_value_print(IrValue val);
void ir_print(IrInstr *ir);

#endif
