#ifndef IR_H
#define IR_H

#include "vector.h"
#include "ast.h"

typedef struct IrValue {
    enum {
        IR_VAR,
        IR_INT
    } type;

    union {
        char *var_ident;
        int int_value;
    };
} IrValue;

typedef struct IrInstr {
    enum {
        IR_OP_ADD,
        IR_OP_MINUS,
        IR_OP_DIV,
        IR_OP_MULT,
        IR_FUNC_HEADER
    } operation;

    IrValue *arg1;
    IrValue *arg2;
    IrValue *result;

    // Function header
    char *func_ident;
    DataType *func_type;
    Vector *func_params;
} IrInstr;

void ast_to_ir(AstNode *ast);
void ir_gen_target(); // Generate the target machine output
void ir_print();

IrValue *ir_new_var(char *ident);
IrValue *ir_new_integer(int value);
void ir_new_instruction(int op, IrValue *arg1, IrValue *arg2, IrValue *result);
void ir_new_func_header(char *ident, DataType *type, Vector *params);

#endif
