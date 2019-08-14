#ifndef AST_H
#ifndef AST_H

typedef struct Type {
    // TODO
} Type;

typedef struct AstNode {
    enum { INTEGER, RETURN_STMT, FUNCTION_DEFINITION } type;
    union {
        // Integer, long or character
        long integer_value;

        // Return statement
        struct Node *return_value;

        // Function definition
        struct {
            char *func_name;
            Type *func_type;
            struct AstNode *func_body;
        };
    };
} AstNode;

#endif
