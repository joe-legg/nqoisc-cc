#include "ir.h"
#include "malloc_or_die.h"

IrInstr *new_ir_instr(int type, IrValue p0, IrValue p1,
                      IrInstr *next, IrInstr *branch)
{
    IrInstr *instr = malloc_or_die(sizeof(IrInstr));
    instr->type = type;
    instr->p0 = p0;
    instr->p1 = p1;
    instr->next = next;
    instr->branch = branch;
    return instr;
}
