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

AstNode *new_ast_conditional(int node_type, AstNode *cond, AstNode *cond_body,
                             AstNode *cond_else)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = node_type;
    node->cond = cond;
    node->cond_body = cond_body;
    node->cond_else = cond_else;
    return node;
}

DataType *new_data_type(int type, int is_unsigned, int storage_specs,
                        int type_qualifiers)
{
    DataType *new_type = malloc(sizeof(DataType));
    new_type->type = type;
    new_type->is_unsigned = is_unsigned;
    new_type->storage_specs = storage_specs;
    new_type->type_qualifiers = type_qualifiers;
    return new_type;
}

/* Delete AST */

void delete_data_type(DataType *type) { free(type); }

void delete_ast(AstNode *ast)
{
    switch (ast->node_type) {
        case AST_IDENTIFIER:    free(ast->identifier); break;
        case AST_EXPR_STMT:     delete_ast(ast->expression); break;
        case AST_RETURN_STMT:   delete_ast(ast->return_expr); break;
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
            for (int i = 0; i < ast->statements->length; i++)
                delete_ast(ast->statements->items[i]);
            vector_free(ast->statements);
            break;
        // Conditionals
        case AST_IF_STMT:
        case AST_WHILE_STMT:
        case AST_DO_WHILE_STMT:
            delete_ast(ast->cond);
            delete_ast(ast->cond_body);
            if (ast->cond_else != NULL)
                delete_ast(ast->cond_else);
            break;
        case AST_FUNCTION_DEF:
            for (int i = 0; i < ast->func_params->length; i++)
                delete_ast(ast->func_params->items[i]);
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
    char *sign_str = type->is_unsigned ? "unsigned" : "signed";
    char *type_spec_str;
    char type_qualifier[25] = { 0 };
    char type_str[25];

    // Storage specifiers
    switch (type->storage_specs) {
    case STORAGE_SPEC_TYPEDEF:  /* TODO */ break;
    case STORAGE_SPEC_EXTERN:   type_spec_str = "extern "; break;
    case STORAGE_SPEC_STATIC:   type_spec_str = "static "; break;
    case STORAGE_SPEC_AUTO:     type_spec_str = "auto "; break;
    case STORAGE_SPEC_REGISTER: type_spec_str = "register "; break;
    default: type_spec_str = "";
    }

    // Type qualifiers
    if (type->type_qualifiers & TYPE_QUAL_CONST)
        strcat(type_qualifier, "const ");
    if (type->type_qualifiers & TYPE_QUAL_RESTRICT)
        strcat(type_qualifier, "restrict ");
    if (type->type_qualifiers & TYPE_QUAL_VOLATILE)
        strcat(type_qualifier, "volatile ");

    // Types
    switch (type->type) {
    case TYPE_VOID:    sprintf(type_str, "void"); break;
    case TYPE_BOOL:    sprintf(type_str, "_Bool"); break;
    case TYPE_FLOAT:   sprintf(type_str, "float"); break;
    case TYPE_DOUBLE:  sprintf(type_str, "double"); break;
    case TYPE_LDOUBLE: sprintf(type_str, "long double"); break;
    case TYPE_INT:     sprintf(type_str, "%s int", sign_str); break;
    case TYPE_CHAR:    sprintf(type_str, "%s char", sign_str); break;
    case TYPE_SHORT:   sprintf(type_str, "%s short", sign_str); break;
    case TYPE_LONG:    sprintf(type_str, "%s long", sign_str); break;
    case TYPE_LLONG:   sprintf(type_str, "%s long long", sign_str); break;
    }

    char final_str[60];
    sprintf(final_str, "%s%s%s", type_spec_str, type_qualifier, type_str);

    return strdup(final_str);
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
        case AST_IF_STMT:
            printf("(cond-if ");
            goto print_cond;
        case AST_WHILE_STMT:
            printf("(cond-while ");
            goto print_cond;
        case AST_DO_WHILE_STMT:
            printf("(cond-do-while ");
        print_cond:
            print_ast(ast->cond);
            printf(" ");
            print_ast(ast->cond_body);
            if (ast->cond_else != NULL) {
                printf(" ");
                print_ast(ast->cond_else);
            }
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
