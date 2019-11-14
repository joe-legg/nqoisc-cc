#ifndef IR_H
#define IR_H

#include "vector.h"
#include "ast.h"

typedef struct IrStmt {
    enum {
        IR_ADD,
        IR_COPY,
        IR_SET,
        IR_BNZ,
        IR_FCALL,
        IR_RET
    } type;

    Vector *prameters;

    struct IrStmt *next;
    struct IrStmt *branch; // Used for loops and conditions
} IrStmt;

IrStmt *ir_from_ast(AstNode *ast);
void free_ir(IrStmt *ir);

void print_ir(IrStmt *ir);

#endif
