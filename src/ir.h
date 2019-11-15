#ifndef IR_H
#define IR_H

#include "vector.h"
#include "ast.h"

// Parameter for and IR statement
typedef union IrParam {
    unsigned int mem_addr;   // Memory address
    long long integer_const; // Constant integer literal
    char *identifier;
} IrParam;

// Single IR statement
typedef struct IrStmt {
    enum {
        // Arithmetic
        IR_ADD,
        // Conditionals
        IR_EQ,
        IR_IF,
        // Variables/memory
        IR_COPY,
        IR_SET,
        // Functions
        IR_FCALL,
        IR_FUNC_HEADER,
        IR_RET
    } type;

    // The IR uses a linked-list-like data structure
    struct IrStmt *next;
    struct IrStmt *branch; // Used for loops, conditions and functions

    // Parameters
    IrParam p0;
    IrParam p1;
    IrParam p2;
} IrStmt;

// IR gen context
typedef struct IrContext {
    int variable_count;
} IrContext;

IrContext *new_ir_context();
IrStmt *ir_from_ast(AstNode *ast, IrContext *ctx);
void free_ir(IrStmt *ir);

void print_ir(IrStmt *ir);

#endif
