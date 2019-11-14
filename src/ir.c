#include "ir.h"
#include <stdio.h>
#include <stdlib.h>

#define ir_for_each(stmt, ir) \
    for (IrStmt *stmt = ir; ir != NULL; ir = ir->next)

IrStmt *ir_from_ast(AstNode *ast)
{
    switch (ast->node_type) {
    case AST_FUNCTION_DEF:
        break;
    case AST_FUNC_DECLARATION:
        break;
    case AST_FUNC_CALL:
        break;
    case AST_RETURN_STMT:
        break;
    case AST_EXPR_STMT:
        break;
    case AST_COMPOUND_STMT:
        break;
    case AST_WHILE_STMT:
        break;
    case AST_DO_WHILE_STMT:
        break;
    case AST_IF_STMT:
        break;
    case AST_GOTO_STMT:
        break;
    case AST_BREAK_STMT:
        break;
    case AST_CONTINUE_STMT:
        break;
    case AST_LABEL_STMT:
        break;
    case AST_SWITCH_STMT:
        break;
    case AST_CASE_STMT:
        break;
    case AST_DEFAULT_STMT:
        break;
    case AST_DECLARATOR_HEAD:
        break;
    case AST_DECLARATION:
        break;
    case AST_TYPEDEF:
        break;
    case AST_DECL_LIST:
        break;
    case AST_CONDITIONAL_EXPR:
        break;
    case AST_STRUCT_MEMBER_ACCESS:
        break;
    case AST_INTEGER_CONST:
        break;
    case AST_FLOAT_CONST:
        break;
    case AST_STR_LIT:
        break;
    case AST_IDENTIFIER:
        break;
    case AST_CAST_EXPR:
        break;
    case AST_BINARY_OP:
        break;
    case AST_UNARY_OP:
        break;
    }
}

void free_ir(IrStmt *ir)
{
    ir_for_each (i, ir) {
        // TODO
    }
}

void print_ir(IrStmt *ir)
{
    // TODO: implement this function
    ir_for_each (i, ir) {
        switch (i->type) {
        case IR_ADD:
            printf("(add)");
            break;
        case IR_COPY:
            printf("(copy)");
            break;
        case IR_SET:
            printf("(set)");
            break;
        case IR_BNZ:
            printf("(bnz)");
            break;
        case IR_FCALL:
            printf("(fcall)");
            break;
        case IR_RET:
            printf("(ret)");
            break;
        }
    }
}
