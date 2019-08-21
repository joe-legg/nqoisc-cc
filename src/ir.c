#include <stdlib.h>
#include <stdio.h>

#include "ir.h"
#include "error.h"

static Vector *ir_instrs; // List of instructions

/* Initializers */

IrValue *ir_new_var(char *ident)
{
    IrValue *val = malloc(sizeof(IrValue));
    val->type = IR_VAR;
    val->var_ident = ident;
    return val;
}

IrValue *ir_new_integer(int value)
{
    IrValue *val = malloc(sizeof(IrValue));
    val->type = IR_INT;
    val->int_value = value;
    return val;
}


void ir_new_func_header(char *ident, DataType *type, Vector *params)
{
    IrInstr *instr = malloc(sizeof(IrInstr));
    instr->operation = IR_FUNC_HEADER;
    instr->func_ident = ident;
    instr->func_type = type;
    instr->func_params = params;
    vector_append(ir_instrs, instr);
}

void ir_new_instruction(int op, IrValue *arg1, IrValue *arg2, IrValue *result)
{
    IrInstr *instr = malloc(sizeof(IrInstr));
    instr->operation = op;
    instr->arg1 = arg1;
    instr->arg2 = arg2;
    instr->result = result;
    vector_append(ir_instrs, instr);
}

/* AST To Ir */

static void gen_ir_from_ast(AstNode *ast)
{
    switch (ast->node_type) {
    case AST_FUNCTION_DEF:
        // TODO: add support for function parameters
        ir_new_func_header(ast->func_ident, ast->func_type, new_vector());
        gen_ir_from_ast(ast->func_body);
        break;
    case AST_RETURN_STMT:
        // TODO
        break;
    case AST_EXPR_STMT:
        // TODO
        break;
    case AST_COMPOUND_STMT:
        // TODO
        break;
    case AST_INTEGER_CONST:
        // TODO
        break;
    case AST_IDENTIFIER:
        // TODO
        break;
    case AST_BINARY_OP:
        // TODO
        break;
    default:
        warning("-", -1, "unkown ast type in IR generation. You should never be "
                        "seeing this warning.");
    }
}

void ast_to_ir(AstNode *ast)
{
    ir_instrs = new_vector();
    gen_ir_from_ast(ast);
}

/* Printing */

static void print_value(IrValue *val)
{
    switch (val->type) {
    case IR_VAR:
        printf("%s", val->var_ident);
        break;
    case IR_INT:
        printf("%d", val->int_value);
        break;
    }
}

static void print_formatted_ir(IrInstr *instr, char *op_str)
{
    printf("(set ");
    print_value(instr->result);
    printf(" (%s ", op_str);
    print_value(instr->arg1);
    printf(" ");
    print_value(instr->arg2);
    printf("))\n");
}

void ir_print()
{
    for (int i = 0; i < ir_instrs->length; i++) {
        IrInstr *cur_instr = ir_instrs->items[i];
        switch (cur_instr->operation) {
        case IR_OP_ADD:
            print_formatted_ir(cur_instr, "add");
            break;
        case IR_OP_MINUS:
            print_formatted_ir(cur_instr, "minus");
            break;
        case IR_OP_DIV:
            print_formatted_ir(cur_instr, "div");
            break;
        case IR_OP_MULT:
            print_formatted_ir(cur_instr, "mult");
            break;
        case IR_FUNC_HEADER:
            printf("(func-head %s ", cur_instr->func_ident);
            print_data_type(cur_instr->func_type);
            printf(")\n");
            break;
        }
    }
}

/* Code Gen */

static void ir_gen_instr(IrInstr *instr)
{
}

void ir_gen_target()
{
    for (int i = 0; i < ir_instrs->length; i++)
        ir_gen_instr(ir_instrs->items[i]);
}
