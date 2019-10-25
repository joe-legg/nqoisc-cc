#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"

/* New Ast Functions */

AstNode *new_expr_stmt(AstNode *expr)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_EXPR_STMT;
    node->expression = expr;
    return node;
}

AstNode *new_ast_ident(const char *ident)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_IDENTIFIER;
    node->identifier = strdup(ident);
    return node;
}

AstNode *new_ast_declaration(DataType *decl_type, AstNode *declarator,
                             AstNode *initializer)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_DECLARATION;
    node->decl_type = decl_type;
    node->decl_declarator = declarator;
    node->decl_initializer = initializer;
    return node;
}

AstNode *new_ast_string_lit(char *str)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_STR_LIT;

    // Remove quotation marks from string. This is needed because of a badly
    // writen regex in lexer.l
    memmove(str, str + 1, strlen(str));
    str[strlen(str) - 1] = '\0';

    node->str_lit = str;
    return node;
}

AstNode *new_ast_integer_const(long value)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_INTEGER_CONST;
    node->integer_const = value;
    return node;
}

AstNode *new_ast_float_const(long double value)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_FLOAT_CONST;
    node->float_const = value;
    return node;
}

AstNode *new_ast_unary_op(int op, AstNode *expression)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_UNARY_OP;
    node->unary_op = op;
    node->unary_expr = expression;
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

AstNode *new_ast_struct_member_access(AstNode *structure,
                                      char *struct_member_ident)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->node_type = AST_STRUCT_MEMBER_ACCESS;
    node->structure = structure;
    node->struct_member_ident = struct_member_ident;
    return node;
}

AstNode *new_ast_for_loop(AstNode *clause_1, AstNode *expr_2, AstNode *expr_3,
                          AstNode *body)
{
    AstNode *stmts = malloc(sizeof(AstNode));
    stmts->node_type = AST_COMPOUND_STMT;
    stmts->statements = new_vector();

    vector_append(stmts->statements, clause_1);

    if (expr_3 != NULL) {
        if (body->statements == NULL) {
            body->statements = new_vector();
            vector_append(body->statements, expr_3);
        }
    }

    vector_append(stmts->statements,
                  new_ast_conditional(AST_WHILE_STMT, expr_2, body, NULL));
    return stmts;
}

AstNode *ast_declarator_head_to_identifier(AstNode *declarator_head)
{
    AstNode *ident = new_ast_ident(declarator_head->declarator_head_ident);
    free_ast(declarator_head);
    return ident;
}

/* Data types */

DataType *new_data_type(int type, int is_unsigned, int storage_specs,
                        int type_qualifiers, DataType *pointer)
{
    DataType *new_type = malloc(sizeof(DataType));
    new_type->type = type;
    new_type->is_unsigned = is_unsigned;
    new_type->storage_specs = storage_specs;
    new_type->type_qualifiers = type_qualifiers;
    new_type->pointer = pointer;
    return new_type;
}

// This function is needed because pointer data types
// are stored as linked lists.
void set_pointer_type(DataType *pointer, DataType *type)
{
    DataType *last_pointer = pointer;
    while (last_pointer->pointer != NULL)
        last_pointer = last_pointer->pointer;
    last_pointer->pointer = type;
}

// Compare data types
int cmp_data_types(DataType *type_a, DataType *type_b)
{
    if (type_a->type == type_b->type                       ||
        type_a->is_unsigned == type_b->is_unsigned         ||
        type_a->storage_specs == type_b->storage_specs     ||
        type_a->type_qualifiers == type_b->type_qualifiers ||
        cmp_data_types(type_a->pointer, type_b->pointer))
        return 1;
    return 0;
}

void free_data_type(DataType *type)
{
    if (type->pointer != NULL) free_data_type(type->pointer);
    free(type);
}

/* Free AST */

void free_ast(AstNode *ast)
{
    // Because of this there is no need to check for null children
    if (ast == NULL) return;

    switch (ast->node_type) {
    case AST_GOTO_STMT:
    case AST_STR_LIT:      free(ast->str_lit); break;
    case AST_IDENTIFIER:   free(ast->identifier); break;
    case AST_DEFAULT_STMT: free_ast(ast->default_stmt); break;
    case AST_EXPR_STMT:
    case AST_RETURN_STMT:  free_ast(ast->expression); break;
    case AST_LABEL_STMT:
        free(ast->label_ident);
        free_ast(ast->label_stmt);
        break;
    case AST_BINARY_OP:
        free_ast(ast->binary_left);
        free_ast(ast->binary_right);
        break;
    case AST_DECLARATOR_HEAD:
        if (ast->declarator_head_pointer != NULL)
            free_data_type(ast->declarator_head_pointer);
        free(ast->declarator_head_ident);
        break;
    case AST_DECL_LIST:
        for (int i = 0; i < ast->decl_list->length; i++)
            free_ast(ast->decl_list->items[i]);
        vector_free(ast->decl_list);
        break;
    case AST_DECLARATION:
        free_data_type(ast->decl_type);
        free_ast(ast->decl_declarator);
        free_ast(ast->decl_initializer);
        break;
    case AST_COMPOUND_STMT:
        for (int i = 0; i < ast->statements->length; i++)
            free_ast(ast->statements->items[i]);
        vector_free(ast->statements);
        break;
    case AST_STRUCT_MEMBER_ACCESS:
        free_ast(ast->structure);
        free(ast->struct_member_ident);
        break;
    // Conditionals
    case AST_IF_STMT:
    case AST_CASE_STMT:
    case AST_WHILE_STMT:
    case AST_SWITCH_STMT:
    case AST_DO_WHILE_STMT:
    case AST_CONDITIONAL_EXPR:
        free_ast(ast->cond);
        free_ast(ast->cond_body);
        free_ast(ast->cond_else);
        break;
    case AST_FUNC_DECLARATION:
    case AST_FUNCTION_DEF:
        for (int i = 0; i < ast->func_params->length; i++)
            free_ast(ast->func_params->items[i]);
        vector_free(ast->func_params);
        free_data_type(ast->func_type);
        free(ast->func_ident);
        free_ast(ast->func_body);
        break;
    case AST_FUNC_CALL:
        free(ast->func_call_ident);
        for (int i = 0; i < ast->func_call_args->length; i++)
            free_ast(ast->func_call_args->items[i]);
        vector_free(ast->func_call_args);
        break;
    case AST_UNARY_OP:
    case AST_CONTINUE_STMT:
    case AST_INTEGER_CONST:
    case AST_FLOAT_CONST:
    case AST_BREAK_STMT:
        break;
    }
    free(ast);
}

/* Print AST */

char *type_to_string(const DataType *type)
{
    char *sign_str = type->is_unsigned ? "unsigned" : "signed";
    char *type_spec_str;
    char type_str[25];
    char type_qualifier[25] = { 0 };

    // If it is a pointer
    if (type->pointer != NULL) {
        // Type qualifiers
        if (type->type_qualifiers & TYPE_QUAL_CONST)
            strcat(type_qualifier, "const ");
        if (type->type_qualifiers & TYPE_QUAL_RESTRICT)
            strcat(type_qualifier, "restrict ");
        if (type->type_qualifiers & TYPE_QUAL_VOLATILE)
            strcat(type_qualifier, "volatile ");

        char *pointer_str = type_to_string(type->pointer);

        char final_str[strlen(pointer_str) + 25];
        sprintf(final_str, "%s%s*", pointer_str, type_qualifier);
        free(pointer_str);
        return strdup(final_str);
    }

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
    case AST_BREAK_STMT:    printf("(break)"); break;
    case AST_CONTINUE_STMT: printf("(continue)"); break;
    case AST_GOTO_STMT:     printf("(goto %s)", ast->identifier); break;
    case AST_STR_LIT:       printf("(string \"%s\")", ast->str_lit); break;
    case AST_IDENTIFIER:    printf("(identifier %s)", ast->identifier); break;
    case AST_FLOAT_CONST:
        printf("(float-const %.10Lf)", ast->float_const);
        break;
    case AST_INTEGER_CONST:
        printf("(integer-const %lli)", ast->integer_const);
        break;
    case AST_LABEL_STMT:
        printf("(label %s ", ast->label_ident);
        print_ast(ast->label_stmt);
        printf(")");
        break;
    case AST_UNARY_OP:
        printf("(unary-op %d ", ast->unary_op);
        print_ast(ast->unary_expr);
        printf(")");
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
        if (ast->expression == NULL) printf("(null)");
        else print_ast(ast->expression);
        printf(")");
        break;
    case AST_DEFAULT_STMT:
        printf("(default-stmt ");
        print_ast(ast->default_stmt);
        printf(")");
        break;
    // Conditionals
    case AST_IF_STMT:          printf("(cond-if-stmt "); goto print_cond;
    case AST_CASE_STMT:        printf("(case-stmt "); goto print_cond;
    case AST_SWITCH_STMT:      printf("(switch-stmt "); goto print_cond;
    case AST_WHILE_STMT:       printf("(cond-while-stmt "); goto print_cond;
    case AST_DO_WHILE_STMT:    printf("(cond-do-while-stmt "); goto print_cond;
    case AST_CONDITIONAL_EXPR: printf("(cond-expr ");
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
            for (int i = 0; i < ast->statements->length; i++) {
                print_ast(ast->statements->items[i]);
                if (i != ast->statements->length - 1) printf(" ");
            }
        }
        printf(")");
        break;
    case AST_STRUCT_MEMBER_ACCESS:
        printf("(struct-member-access \"%s\" ", ast->struct_member_ident);
        print_ast(ast->structure);
        printf(")");
        break;
    case AST_DECLARATION:
        printf("(declaration ");
        print_data_type(ast->decl_type);
        if (ast->decl_declarator != NULL) {
            printf(" ");
            print_ast(ast->decl_declarator);
        }
        if (ast->decl_initializer != NULL) {
            printf(" ");
            print_ast(ast->decl_initializer);
        }
        printf(")");
        break;
    case AST_DECLARATOR_HEAD:
        printf("(declarator-head %s ",
                ast->declarator_head_ident);
        if (ast->declarator_head_pointer != NULL)
            print_data_type(ast->declarator_head_pointer);
        printf(")");
        break;
    case AST_DECL_LIST:
        for (int i = 0; i < ast->decl_list->length; i++) {
            print_ast(ast->decl_list->items[i]);
            if (i != ast->decl_list->length - 1) printf(" ");
        }
        break;
    case AST_FUNC_DECLARATION:
        printf("(func-declaration %s ", ast->func_ident); // Name
        print_data_type(ast->func_type); // Type
        // Print parameters
        printf(" (parameters ");
        if (!ast->func_params->length) {
            printf("(null)");
        } else {
            for (int i = 0; i < ast->func_params->length; i++) {
                print_ast(ast->func_params->items[i]);
                if (i != ast->func_params->length - 1) printf(" ");
            }
        }
        printf("))");
        break;
    case AST_FUNCTION_DEF:
        printf("(function-def %s ", ast->func_ident); // Name
        print_data_type(ast->func_type); // Type
        // Print parameters
        printf(" (parameters ");
        if (!ast->func_params->length) {
            printf("(null)");
        } else {
            for (int i = 0; i < ast->func_params->length; i++) {
                print_ast(ast->func_params->items[i]);
                if (i != ast->func_params->length - 1) printf(" ");
            }
        }
        printf(") ");
        print_ast(ast->func_body); // Body
        printf(")");
        break;
    case AST_FUNC_CALL:
        printf("(func-call %s (args ", ast->func_call_ident);
        if (ast->func_call_args != NULL) {
            for (int i = 0; i < ast->func_call_args->length; i++) {
                print_ast(ast->func_call_args->items[i]);
                if (i != ast->func_call_args->length - 1) printf(" ");
            }
        }
        printf("))");
        break;
    }
}
