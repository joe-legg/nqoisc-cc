#ifndef AST_H
#define AST_H

#include "vector.h"

enum {
    TYPE_INT
};

typedef struct DataType {
    int type;
} DataType;

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
        AST_IDENTIFIER
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

DataType *new_data_type(int type);

void print_ast(AstNode *ast);

#endif
