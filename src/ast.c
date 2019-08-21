#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"

/* New Ast Functions */

AstNode *new_ast_ident(const char *ident)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_IDENTIFIER;
    node->identifier = strdup(ident);
    return node;
}

AstNode *new_ast_integer_const(long value)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_INTEGER_CONST;
    node->integer_const = value;
    return node;
}

AstNode *new_ast_binary_op(int op, AstNode *left, AstNode *right)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_BINARY_OP;
    node->binary_left = left;
    node->binary_right = right;
    node->binary_op = op;
    return node;
}

DataType *new_data_type(int type)
{
    DataType *new_type = malloc(sizeof(DataType));
    new_type->type = type;
    return new_type;
}

/* Print AST */

void print_data_type(const DataType *type)
{
    printf("(type %d)", type->type);
}

void print_ast(AstNode *ast)
{
    switch (ast->node_type) {
        case AST_INTEGER_CONST:
            printf("(integer-val %ld)", ast->integer_const);
            break;
        case AST_IDENTIFIER:
            printf("(identifier %s)", ast->identifier);
            break;
        case AST_BINARY_OP:
            printf("(binary-op %d ", ast->binary_op);
            print_ast(ast->binary_left);
            printf(" ");
            print_ast(ast->binary_right);
            printf(")");
            break;
        case AST_RETURN_STMT:
            printf("(return-stmt ");
            if (ast->return_expr == NULL) printf("(null)");
            else print_ast(ast->return_expr);
            printf(")");
            break;
        case AST_COMPOUND_STMT:
            printf("(compound-stmt ");
            for (int i = 0; i < ast->statements->length; i++) {
                print_ast(ast->statements->items[i]);
                printf(" ");
            }
            printf(")");
            break;
        case AST_FUNCTION_DEF:
            printf("(function-def %s ", ast->func_ident);
            print_data_type(ast->func_type);
            printf(" ");
            print_ast(ast->func_body);
            printf(")");
            break;
    }
}
