#include "code_gen.h"
#include "error.h"
#include <stdio.h>
#include <stdint.h>

#define GEN_RIGHT(imm) gen_instr(INST_RIGHT, imm)  // Move data ptr right
#define GEN_LEFT(imm)  gen_instr(INST_RIGHT, -imm) // Move data ptr left
#define GEN_ADD(imm)   gen_instr(INST_ADD, imm)    // Add to value at data ptr
#define GEN_SUB(imm)   gen_instr(INST_ADD, -imm)   // Sub from value at data ptr
#define GEN_BNZ(imm)   gen_instr(INST_BNZ, imm)    // Branch if not zero

// Word length
typedef uint32_t Word;

// Instructions
enum {
    INST_RIGHT = 0x0,
    INST_ADD   = 0x1,
    INST_BNZ   = 0x2
};

static FILE *out_file;

static Word instr_addr = -1;

static void gen_instr(char opcode, Word imm)
{
    Word instr = opcode | imm;
    fwrite(&instr, 4, 1, out_file);
    instr_addr++;
}

// Set the value at the data pointer to zero
// Note: If the value at the data pointer is allready zero this causes and
//       integer overflow and can be very inefficient. Possiably consider a 4th
//       instruction for initializing memory addresses to zero.
static void zero_at_dp()
{
    GEN_SUB(1);
    GEN_BNZ(-4);
}

/*
The Stack.

Due to hardware limitations (only having 3 instructions) I thought implementing
a stack was going to be quite a complex task but as it turns out it was simpler
than I first thought.

0x0 |-------| <- Bottom of the stack
    |       |
    | Stack |
0xF |       | <- The next address to be pushed to (This is where the stack
    |-------|    pointer usualy points to).

The data pointer is used as the stack pointer. The stack grows upwards.

Pushing to the stack.
    1. Set the value pointed to by the data pointer to the value to be pushed.
    2. Increment the data pointer by 1.

Poping from the stack.
    1. Decrement the data pointer by 1.
*/

static void stack_push(Word value)
{
    // Set the value
    zero_at_dp();
    GEN_ADD(value);
    // Increment the data pointer
    GEN_RIGHT(1);
}

static void stack_pop()
{
    GEN_LEFT(1);
}

/* Function Definition */

static void function_prologue(AstNode *ast)
{
    // TODO
    // Save old frame pointer
    stack_push(
    // Create new frame pointer
}

static void function_epilogue()
{
    // TODO
    // Pop stack frame
    // Push return value to the stack
    // Jump to calling location
}

static void compile_ast(AstNode *ast)
{
    switch (ast->node_type) {
        case AST_FUNCTION_DEF:
            function_prologue(ast);
            compile_ast(ast->func_body);
            function_epilogue();
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
            for (int i = 0; i < ast->statements->length; i++)
                compile_ast(ast->statements->items[i]);
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

void code_gen(AstNode *ast, const char *out_filename)
{
    out_file = fopen(out_filename, "wb");

    if (out_filename == NULL)
        error("", -1, "failed to open output file.");

    compile_ast(ast);

    fclose(out_file);
}
