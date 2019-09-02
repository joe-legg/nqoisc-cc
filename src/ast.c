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

AstNode *new_ast_declaration(DataType *decl_type, AstNode *declarator)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_DECLARATION;
    node->declaration_type = decl_type;
    node->declaration_declarator = declarator;
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

DataType *new_data_type(int type, int is_unsigned, int storage_specs)
{
    DataType *new_type = malloc(sizeof(DataType));
    new_type->type = type;
    new_type->is_unsigned = is_unsigned;
    new_type->storage_specs = storage_specs;
    return new_type;
}

/* Delete AST */

void delete_data_type(DataType *type)
{
    free(type);
}

void delete_ast(AstNode *ast)
{
    switch (ast->node_type) {
        case AST_IDENTIFIER:  free(ast->identifier); break;
        case AST_EXPR_STMT:   delete_ast(ast->expression); break;
        case AST_RETURN_STMT: delete_ast(ast->return_expr); break;
        case AST_INTEGER_CONST: break;
        case AST_BINARY_OP:
            delete_ast(ast->binary_left);
            delete_ast(ast->binary_right);
            break;
        case AST_DECLARATION:
            delete_data_type(ast->declaration_type);
            delete_ast(ast->declaration_declarator);
            break;
        case AST_COMPOUND_STMT:
            for (int i = 0; i < ast->statements->length; i++) {
                delete_ast(ast->statements->items[i]);
            }
            vector_free(ast->statements);
            break;
        case AST_FUNCTION_DEF:
            for (int i = 0; i < ast->func_params->length; i++) {
                delete_ast(ast->func_params->items[i]);
            }
            vector_free(ast->func_params);
            delete_data_type(ast->func_type);
            free(ast->func_ident);
            delete_ast(ast->func_body);
            break;
    }
    free(ast);
}

/* Print AST */

char *type_to_string(const DataType *type)
{
    // TODO: pointers
    char *sign_str = type->is_unsigned ? "unsigned" : "signed";
    char type_string[50];
    char storage_specs_str[10];

    switch (type->storage_specs) {
    case STORAGE_SPEC_TYPEDEF:  sprintf(storage_specs_str, "typedef "); break;
    case STORAGE_SPEC_EXTERN:   sprintf(storage_specs_str, "extern "); break;
    case STORAGE_SPEC_STATIC:   sprintf(storage_specs_str, "static "); break;
    case STORAGE_SPEC_AUTO:     sprintf(storage_specs_str, "auto "); break;
    case STORAGE_SPEC_REGISTER: sprintf(storage_specs_str, "register "); break;
    //default: sprintf(storage_specs_str, ""); break;
    }

    switch (type->type) {
    case TYPE_INT:
        sprintf(type_string, "%s%s int", storage_specs_str, sign_str); break;
    case TYPE_CHAR:
        sprintf(type_string, "%s%s char", storage_specs_str, sign_str); break;
    case TYPE_LLONG:
        sprintf(type_string, "%s%s long long", storage_specs_str, sign_str);
        break;
    case TYPE_LONG:
        sprintf(type_string, "%s%s long", storage_specs_str, sign_str); break;
    case TYPE_SHORT:
        sprintf(type_string, "%s%s short", storage_specs_str, sign_str); break;
    case TYPE_VOID:
        sprintf(type_string, "%svoid", storage_specs_str); break;
    case TYPE_FLOAT:
        sprintf(type_string, "%sfloat", storage_specs_str); break;
    case TYPE_DOUBLE:
        sprintf(type_string, "%sdouble", storage_specs_str); break;
    case TYPE_LDOUBLE:
        sprintf(type_string, "%slong double", storage_specs_str); break;
    case TYPE_BOOL:
        sprintf(type_string, "%s_Bool", storage_specs_str); break;
    case TYPE_SIGNED:
        sprintf(type_string, "%ssigned", storage_specs_str); break;
    case TYPE_UNSIGNED:
        sprintf(type_string, "%sunsigned", storage_specs_str); break;
    case TYPE_POINTER:
        // TODO
        break;
    }

    return strdup(type_string);
}

static void print_data_type(const DataType *type)
{
    char *type_str = type_to_string(type);
    printf("(type \"%s\")", type_str);
    free(type_str);
}

void print_ast(AstNode *ast)
{
    switch (ast->node_type) {
        case AST_INTEGER_CONST:
            printf("(integer-val %lli)", ast->integer_const);
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
        case AST_EXPR_STMT:
            printf("(expr-stmt ");
            print_ast(ast->expression);
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
            if (!ast->statements->length) {
                printf("(null)");
            } else {
                print_ast(ast->statements->items[0]);
                for (int i = 1; i < ast->statements->length; i++) {
                    printf(" ");
                    print_ast(ast->statements->items[i]);
                }
            }
            printf(")");
            break;
        case AST_DECLARATION:
            printf("(declaration ");
            print_data_type(ast->declaration_type);
            if (ast->declaration_declarator != NULL) {
                printf(" ");
                print_ast(ast->declaration_declarator);
            }
            printf(")");
            break;
        case AST_FUNCTION_DEF:
            printf("(function-def %s ", ast->func_ident); // Name
            print_data_type(ast->func_type); // Type
            // Print parameters
            printf(" (parameters ");
            if (!ast->func_params->length) {
                printf("(null)");
            } else {
                print_ast(ast->func_params->items[0]);
                for (int i = 1; i < ast->func_params->length; i++) {
                    printf(" ");
                    print_ast(ast->func_params->items[i]);
                }
            }
            printf(") ");
            print_ast(ast->func_body); // Body
            printf(")");
            break;
    }
}
