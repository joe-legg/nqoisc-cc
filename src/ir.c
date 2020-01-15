#include "ir.h"
#include "malloc_or_die.h"
#include <stdio.h>
#include <string.h>

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
    case VALUE_VARIABLE: printf("M%d", val.variable); break;
    case VALUE_INTEGER:  printf("%lld", val.integer); break;
    case VALUE_FLOAT:    printf("%F", val.floating_point); break;
    case VALUE_STRING:   printf("\"%s\"", val.string); break;
    }
}

void ir_print(IrInstr *ir)
{
    // Assign labels
    // Note: I am aware that this is probably not the most optimal code but
    //       as this function only exists for debugging purposes it does not
    //       matter that much.
    int label = 1;
    IrInstr *tmp = ir;
    while (1) {
        if (tmp->branch != NULL) {
            if (tmp->branch->print_label != 0)
                tmp->branch->print_label = label++;
        }

        if (tmp->next == NULL) break;
        tmp = tmp->next;
    }

    // Print AST
    while (1) {
        if (ir->print_label)
            printf("L%d:\n", ir->print_label);

        switch (ir->type) {
        case IR_ADD:   printf("add "); goto print_params;
        case IR_SUB:   printf("sub "); goto print_params;
        case IR_MULT:  printf("mult "); goto print_params;
        case IR_DIV:   printf("div "); goto print_params;
        case IR_INIT:  printf("init "); goto print_params;
        print_params:
            ir_value_print(ir->p0);
            printf(" ");
            ir_value_print(ir->p1);
            break;
        case IR_RET:
            printf("ret ");
            ir_value_print(ir->p0);
            break;
        case IR_JAL: printf("jal L%d", ir->branch->print_label); break;
        case IR_JNZ: printf("jnz L%d", ir->branch->print_label); break;
        case IR_NEW_FRAME: printf("newframe"); break;
        case IR_POP_FRAME: printf("popframe"); break;
        }

        printf("\n");

        if (ir->next == NULL) break;
        ir = ir->next;
    }
}
