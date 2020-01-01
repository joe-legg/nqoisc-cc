#include "ir.h"
#include "malloc_or_die.h"
#include <stdio.h>

IrInstr *new_ir_instr(int type, IrValue p0, IrValue p1,
                      IrInstr *next, IrInstr *branch)
{
    IrInstr *ir = malloc_or_die(sizeof(IrInstr));
    ir->type = type;
    ir->p0 = p0;
    ir->p1 = p1;
    ir->next = next;
    ir->branch = branch;
    ir->print_label = 0; // 0 for no label
    return ir;
}

void free_ir_instr(IrInstr *ir)
{
    if (ir->next != NULL)
        free(ir->next);
    free(ir);
}

void ir_value_print(IrValue val)
{
    switch (val.type) {
    case VALUE_MEM_ADDR: printf("M%d", val.mem_addr); break;
    case VALUE_INTEGER:  printf("%lld", val.integer); break;
    case VALUE_FLOAT:    printf("%LF", val.floating_point); break;
    case VALUE_STRING:   printf("\"%s\"", val.string); break;
    }
}

void ir_print(IrInstr *ir)
{
    int label = 1; // Gets incremented every time a new label is created

    while (1) {
        if (ir->print_label)
            printf("L%d:\n", ir->print_label);

        switch (ir->type) {
        case IR_OP_ADD:  printf("add "); goto print_params;
        case IR_OP_SUB:  printf("sub "); goto print_params;
        case IR_OP_MULT: printf("mult "); goto print_params;
        case IR_OP_DIV:  printf("div "); goto print_params;
        case IR_INIT:    printf("init "); goto print_params;
        case IR_COPY:    printf("copy ");
        print_params:
            ir_value_print(ir->p0);
            printf(" ");
            ir_value_print(ir->p1);
            break;
        case IR_RET:
            printf("return ");
            ir_value_print(ir->p0);
            break;
        case IR_JAL:
            // Assign new label to branch if no label exists
            if (!ir->branch->print_label)
                ir->branch->print_label = label++;
            printf("jal L%d", ir->branch->print_label);
            break;
        case IR_JNZ:
            if (!ir->branch->print_label)
                ir->branch->print_label = label++;
            printf("jnz L%d", ir->branch->print_label);
            break;
        }
        printf("\n");

        if (ir->next == NULL)
            break;
        ir = ir->next;
    }
}
