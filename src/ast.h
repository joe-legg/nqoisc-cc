#ifndef AST_H
#define AST_H

#include "vector.h"

enum {
    TYPE_INT
};

typedef struct DataType {
    int type;
} DataType;

enum {
    OP_PLUS,
    OP_MINUS,
    UNARY_PLUS,
    UNARY_MINUS,
    OP_MULT,
    OP_DIV,
    OP_MODULO,
    UNARY_INC,
    UNARY_DEC,
    OP_EQUALITY,
    OP_NOT_EQUAL,
    OP_GREATER_THAN,
    OP_LESS_THAN,
    OP_LESS_THAN_EQUAL,
    OP_GREATER_THAN_EQUAL,
    UNARY_NOT,
    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_BITWISE_NOT,
    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_BITWISE_AND,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
    OP_ASSIGN_ADD,
    OP_ASSIGN_MINUS,
    OP_ASSIGN_MULT,
    OP_ASSIGN_DIV,
    OP_ASSIGN_MODULO,
    OP_ASSIGN_AND,
    OP_ASSIGN_OR,
    OP_ASSIGN_XOR,
    OP_ASSIGN_SHIFT_LEFT,
    OP_ASSIGN_SHIFT_RIGHT
};

typedef struct AstNode {
    enum {
        // Definitions
        AST_FUNCTION_DEF,

        // Statements
        AST_RETURN_STMT,
        AST_EXPR_STMT,
        AST_COMPOUND_STMT,

        // Expressions
        AST_INTEGER_CONST,
        AST_IDENTIFIER,
        AST_BINARY_OP
    } node_type;

    union {
        // Integer, long or character
        long integer_const;

        // Identifier
        char *identifier;

        // Return statement
        struct AstNode *return_expr;

        // Exprssion statement
        struct AstNode *expression;

        // Compound statement
        Vector *statements;

        // Binary operator
        struct {
            struct AstNode *binary_left;
            struct AstNode *binary_right;
            int binary_op;
        };

        // Function definition
        struct {
            char *func_ident;
            DataType *func_type;
            struct AstNode *func_body;
        };
    };
} AstNode;

AstNode *new_ast_ident(const char *ident);
AstNode *new_ast_integer_const(long value);
AstNode *new_ast_binary_op(int op, AstNode *left, AstNode *right);

DataType *new_data_type(int type);

void print_data_type(const DataType *type);
void print_ast(AstNode *ast);

#endif
