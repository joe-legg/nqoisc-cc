#include <stdio.h>

#include "ast.h"
#include "ir.h"

extern AstNode *parse(char *filename);

int main(int argc, char *argv[])
{
    AstNode *ast;
    if (argc == 2) ast = parse(argv[1]);
    else return 0;
    print_ast(ast);
    printf("\nAST printing done.\n");
    free_ast(ast);

    IrInstr *ir = new_ir_instr(IR_INIT,
        (IrValue) { .type = VALUE_MEM_ADDR, .mem_addr = 1 },
        (IrValue) { .type = VALUE_INTEGER, .integer = 2},
        new_ir_instr(IR_RET, (IrValue) { .type = VALUE_STRING, .string = "test" }, (IrValue) {}, NULL, NULL), NULL);
    ir_print(ir);
    free_ir_instr(ir);
}
