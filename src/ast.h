#ifndef AST_H
#define AST_H

#include "vector.h"

// Data types and declaration specifiers
enum {
    // Types must be ordered by integer conversion rank. Lowest to highest.
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_LLONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LDOUBLE,
    TYPE_ARRAY,
    TYPE_POINTER,

    TYPE_SIGNED,   // TODO: remove signed and unsigned types
    TYPE_UNSIGNED,

    // Storage specifiers
    STORAGE_SPEC_TYPEDEF,
    STORAGE_SPEC_EXTERN,
    STORAGE_SPEC_STATIC,
    STORAGE_SPEC_AUTO,
    STORAGE_SPEC_REGISTER,

    // Type qualifiers bit mask
    TYPE_QUAL_CONST    = 0xff0000,
    TYPE_QUAL_RESTRICT = 0x00ff00,
    TYPE_QUAL_VOLATILE = 0x0000ff
};

typedef struct DataType {
    int type;
    int is_unsigned; // 1 if unsigned
    int storage_specs; // 0 if there are no storage_specs
    int type_qualifiers;

    // Pointers (and arrays) are stored as a linked-list-like data structure.
    // This is probably not the best way to do it but it was
    // the best I could think of.
    struct DataType *pointer;

    // NULL if no array size is specified
    struct AstNode *array_expr;
} DataType;

// Operators for the AST
enum {
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    OP_MODULO,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER_THAN,
    OP_LESS_THAN,
    OP_LESS_THAN_EQUAL,
    OP_GREATER_THAN_EQUAL,
    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_BITWISE_NOT,
    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_BITWISE_AND,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
    OP_ASSIGN_EQUAL,
    OP_ASSIGN_MINUS,
    OP_ASSIGN_PLUS,
    OP_ASSIGN_MULT,
    OP_ASSIGN_DIV,
    OP_ASSIGN_MODULO,
    OP_ASSIGN_AND,
    OP_ASSIGN_OR,
    OP_ASSIGN_XOR,
    OP_ASSIGN_SHIFT_LEFT,
    OP_ASSIGN_SHIFT_RIGHT,
    OP_COMMA,
    UNARY_SIZEOF,
    UNARY_NOT,
    UNARY_INC, // TODO: remove increment and decrement
    UNARY_DEC,
    UNARY_PLUS,
    UNARY_MINUS,
    UNARY_ADDRESS,
    UNARY_DEREF,
    UNARY_BITWISE_NOT
};

typedef struct AstNode {
    enum {
        // Definitions
        AST_FUNCTION_DEF,
        AST_FUNC_DECLARATION,
        AST_FUNC_CALL,

        // Statements
        AST_RETURN_STMT,
        AST_EXPR_STMT,
        AST_COMPOUND_STMT,
        AST_WHILE_STMT,
        AST_DO_WHILE_STMT,
        AST_IF_STMT,
        AST_GOTO_STMT,
        AST_BREAK_STMT,
        AST_CONTINUE_STMT,
        AST_LABEL_STMT,
        AST_SWITCH_STMT,
        AST_CASE_STMT,
        AST_DEFAULT_STMT,

        // Declarations
        AST_DECLARATION,
        AST_DECLARATOR_HEAD,
        AST_TYPEDEF,
        AST_DECL_LIST,

        // Expressions
        AST_CONDITIONAL_EXPR,
        AST_STRUCT_MEMBER_ACCESS,
        AST_INTEGER_CONST,
        AST_FLOAT_CONST,
        AST_STR_LIT,
        AST_IDENTIFIER,
        AST_CAST_EXPR,
        AST_BINARY_OP,
        AST_UNARY_OP,

        // Other
        AST_DATA_TYPE
    } node_type;

    union {
        // Integer, long or character
        long long integer_const;

        // Floating point and double constants
        long double float_const;

        // String literals
        char *str_lit;

        // Identifier, goto
        char *identifier;

        // Exprssion statement
        struct AstNode *expression;

        // Default for switch
        struct AstNode *default_stmt;

        // Compound statement
        Vector *statements;

        // Simple data type wrapper. Only for use in the parser
        // TODO: currently only used for arrays so maybe rename to array_tail
        DataType *data_type;

        // Declarator head. Only used in the parser
        struct {
            char *declarator_head_ident;
            DataType *declarator_head_pointer; // Pointer/array
        };

        // Binary operator
        struct {
            struct AstNode *binary_left;
            struct AstNode *binary_right;
            int binary_op;
        };

        // Unary operator
        struct {
            struct AstNode *unary_expr;
            int unary_op;
        };

        // Cast Expression
        struct {
            DataType *cast_type;
            struct AstNode *cast_expr;
        };

        // Declaration, typedef
        struct {
            DataType *decl_type;
            struct AstNode *decl_declarator;
            struct AstNode *decl_initializer;
        };

        // Declaration list
        Vector *decl_list;

        // Function definition and declaration
        struct {
            char *func_ident;
            DataType *func_type;
            Vector *func_params;
            struct AstNode *func_body;
        };

        // Function call
        struct {
            char *func_call_ident;
            Vector *func_call_args;
        };

        // Conditional: if, while, switch, case
        struct {
            struct AstNode *cond;
            struct AstNode *cond_body;
            struct AstNode *cond_else;
        };

        // Label
        struct {
            char *label_ident;
            struct AstNode *label_stmt;
        };

        // Structure or union member access
        struct {
            struct AstNode *structure;
            char *struct_member_ident;
        };
    };
} AstNode;

// New AST Functions
AstNode *new_ast_string_lit(char *str);
AstNode *new_ast_expr_stmt(AstNode *expr);
AstNode *new_ast_ident(const char *ident);
AstNode *new_ast_integer_const(long value);
AstNode *new_ast_float_const(long double value);
AstNode *new_ast_data_type(DataType *data_type);
AstNode *new_ast_unary_op(int op, AstNode *expression);
AstNode *new_ast_struct_member_access(AstNode *structure,
                                      char *struct_member_ident);
AstNode *new_ast_binary_op(int op, AstNode *left, AstNode *right);
AstNode *new_ast_declaration(DataType *decl_type, AstNode *declarator,
                             AstNode *initializer);
AstNode *new_ast_conditional(int node_type, AstNode *cond, AstNode *cond_body,
                             AstNode *cond_else);
AstNode *new_ast_for_loop(AstNode *clause_1, AstNode *expr_2, AstNode *expr_3,
                          AstNode *body);

// New Data Type
DataType *new_data_type(int type, int is_unsigned, int storage_specs,
                        int type_qualifiers, DataType *pointer,
                        AstNode *array_expr);
void set_pointer_type(DataType *pointer, DataType *type);

// Free functions
void free_ast(AstNode *ast);
void free_data_type(DataType *type);

// Printing
char *type_to_string(const DataType *type); // Used for error messages
void print_ast(AstNode *ast);

#endif
