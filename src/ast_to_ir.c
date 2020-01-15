#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "error.h"
#include "ast_to_ir.h"

// IrValue Macros
#define NEW_VAR_VAL(x) (IrValue) { .type = VALUE_VARIABLE, .variable = x }
#define NEW_INT(x)     (IrValue) { .type = VALUE_INTEGER, .integer = x }
#define NEW_FLOAT(x)   (IrValue) { .type = VALUE_FLOAT, .floating_point = x }
#define NEW_STRING(x)  (IrValue) { .type = VALUE_STRING, .string = x }
#define EMPTY_VALUE    (IrValue) {}

typedef struct IrGenCtx {
    IrInstr *head;
    IrInstr *last; // Pointer to the last ir instruction

    // The name of the last variable referenced
    int last_variable;
} IrGenCtx;

static IrGenCtx ctx_cat(IrGenCtx a, IrGenCtx b)
{
    a.last->next = b.head;
    a.last = b.last;
    return a;
}

static void ctx_append_ir(IrGenCtx *ctx, IrInstr *ir)
{
    ctx->last->next = ir;
    ctx->last = ctx->last->next;
}

static IrGenCtx gen_ir(IrGenCtx ctx, AstNode *ast)
{
    switch (ast->node_type) {
    case AST_FUNCTION_DEF:
        // TODO: finish this
        ctx = ctx_cat(ctx, gen_ir(ctx, ast->func_body));
        break;
    case AST_FUNC_DECLARATION:
        break;
    case AST_FUNC_CALL:
        break;
    case AST_RETURN_STMT:
        ctx = ctx_cat(ctx, gen_ir(ctx, ast->expression));
        ctx_append_ir(&ctx,
                new_ir_instr(IR_RET, NEW_VAR_VAL(ctx.last_variable),
                        EMPTY_VALUE, NULL, NULL));
        break;
    case AST_EXPR_STMT:
        break;
    case AST_COMPOUND_STMT:
        for (int i = 0; i < ast->statements->length; i++)
            ctx = ctx_cat(ctx, gen_ir(ctx, ast->statements->items[i]));
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
        ctx_append_ir(&ctx,
                new_ir_instr(IR_INIT, NEW_VAR_VAL(ctx.last_variable++),
                        NEW_INT(ast->integer_const), NULL, NULL));
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
    // Ignore
    case AST_DECLARATOR_HEAD:
    case AST_DATA_TYPE:
        break;
    }

    printf("print: \n");
    ir_print(ctx.head);
    printf("\n");

    return ctx;
}

IrInstr *ast_to_ir(AstNode *top_level)
{
    // Create context with a placeholder instruction
    IrGenCtx ctx = (IrGenCtx) {
            .head = new_ir_instr(IR_ADD, EMPTY_VALUE, EMPTY_VALUE, NULL, NULL),
            .last = ctx.head,
            .last_variable = 0
    };

    IrInstr *ir = gen_ir(ctx, top_level).head;

    // Remove the placeholder instruction
    IrInstr *placeholder = ir;
    ir = ir->next;
    free(placeholder);

    return ir;
}
