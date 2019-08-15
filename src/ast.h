#ifndef AST_H
#define AST_H

#include "vector.h"

typedef struct Type {
    // TODO
} Type;

typedef struct AstNode {
    enum {
        INTEGER_VALUE,
        RETURN_STMT,
        FUNCTION_DEFINITION,
        COMPOUND_STATEMENT
    } type;

    union {
        // Integer, long or character
        long integer_value;

        // Return statement
        struct Node *return_value;

        // Compound statement
        Vector *statements;

        // Function definition
        struct {
            char *func_name;
            Type *func_type;
            struct AstNode *func_body;
        };
    };
} AstNode;

#endif
