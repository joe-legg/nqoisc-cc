#include "ir.h"
#include "malloc_or_die.h"

#include <stdio.h>
#include <stdlib.h>

#define IR_FOR_EACH(stmt, ir) \
    for (IrStmt *stmt = ir; ir != NULL; ir = ir->next)

/* Parameter Macros */

#define PARAM_ADDR(addr)       (IrParam){ .mem_addr = 1 }
#define PARAM_INTEGER(value)   (IrParam){ .integer_const = value }
#define PARAM_IDENTIFER(ident) (IrParam){ .identifier = ident }
#define EMPTY_PARAM            (IrParam){ }

/* IR Instruction Macros */

#define NEW_IR_SET(dest_addr, value) \
    new_ir_stmt(IR_SET, NULL, NULL, dest_addr, value, EMPTY_PARAM)

static IrStmt *new_ir_stmt(int type, IrStmt *next_stmt, IrStmt *branch_stmt,
                           IrParam p0, IrParam p1, IrParam p2)
{
    IrStmt *stmt = malloc_or_die(sizeof(IrStmt));
    stmt->type = type;
    stmt->next = next_stmt;
    stmt->branch = branch_stmt;
    stmt->p0 = p0;
    stmt->p1 = p1;
    stmt->p2 = p2;
    return stmt;
}

IrContext *new_ir_context()
{
    IrContext *ctx = malloc(sizeof(IrContext));
    ctx->variable_count = 0;
    return ctx;
}

IrStmt *ir_from_ast(AstNode *ast, IrContext *ctx)
{
    switch (ast->node_type) {
    case AST_CONDITIONAL_EXPR:
        break;
    case AST_INTEGER_CONST:
        return NEW_IR_SET(PARAM_ADDR(ctx->variable_count++),
                          PARAM_INTEGER(ast->integer_const));
        break;
    case AST_FLOAT_CONST:
        break;
    case AST_STR_LIT:
        break;
    case AST_IDENTIFIER:
        break;
    case AST_BINARY_OP:
        break;
    case AST_UNARY_OP:
        break;
    case AST_CAST_EXPR:
        break;
    case AST_STRUCT_MEMBER_ACCESS:
        break;
    case AST_FUNCTION_DEF:
        // TODO: function parameters
        return new_ir_stmt(IR_FUNC_HEADER, NULL,
                           ir_from_ast(ast->func_body, ctx),
                           PARAM_IDENTIFER(ast->func_ident),
                           EMPTY_PARAM, EMPTY_PARAM);
        break;
    case AST_FUNC_DECLARATION:
        break;
    case AST_FUNC_CALL:
        break;
    case AST_RETURN_STMT:
        break;
    case AST_EXPR_STMT:
        return ir_from_ast(ast->expression, ctx);
        break;
    case AST_COMPOUND_STMT: {
        if (ast->statements->length > 0) {
            IrStmt *first_stmt = ir_from_ast(ast->statements->items[0], ctx);
            IrStmt *last_stmt = first_stmt;
            for (int i = 1; i < ast->statements->length; i++) {
                last_stmt->next = ir_from_ast(ast->statements->items[i], ctx);
                last_stmt = last_stmt->next;
            }
            return first_stmt;
        }
        return NULL; // Empty compound statement
        break;
    }
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
    }
}

void free_ir(IrStmt *ir)
{
    IR_FOR_EACH (i, ir) {
        // TODO
    }
}

void print_ir(IrStmt *ir)
{
    IR_FOR_EACH (i, ir) {
        switch (i->type) {
        case IR_ADD:
            printf("(add)");
            break;
        case IR_COPY:
            printf("(copy)");
            break;
        case IR_SET:
            // TODO: Implement this properly. Change integer_const to something
            //       else.
            printf("(set m%d %d)", ir->p0.mem_addr, ir->p1.integer_const);
            break;
        case IR_IF:
            printf("(if)");
            break;
        case IR_FUNC_HEADER:
            printf("(func-header %s\n", ir->p0.identifier);
            print_ir(ir->branch);
            printf(")");
            break;
        case IR_FCALL:
            printf("(func-call)");
            break;
        case IR_RET:
            printf("(ret)");
            break;
        }
        printf("\n");
    }
}
