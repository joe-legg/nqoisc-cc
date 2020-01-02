#include "ast_to_ir.h"

#define NEW_MEM_ADDR(x) (IrValue) { .type = VALUE_MEM_ADDR, .mem_addr = x }
#define NEW_INT(x)    (IrValue) { .type = VALUE_INTEGER, .integer = x }
#define NEW_FLOAT(x)  (IrValue) { .type = VALUE_FLOAT, .floating_point = x }
#define NEW_STRING(x) (IrValue) { .type = VALUE_STRING, .string = x }
#define EMPTY_VALUE (IrValue) {}

IrInstr *ast_to_ir(AstNode *ast)
{
    switch(ast->node_type) {
    case AST_COMPOUND_STMT:
        for (int i = 0; i < ast->statements->length; i++)
            ast_to_ir(ast->statements->items[i]);
        break;
    case AST_FUNCTION_DEF:
        // TODO: Parameters, return
        ast_to_ir(ast->func_body);
        break;
    case AST_FUNC_DECLARATION:
        break;
    case AST_FUNC_CALL:
        break;
    case AST_RETURN_STMT:
        // TODO
        break;
    case AST_EXPR_STMT:
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
