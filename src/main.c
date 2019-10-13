#include <stdio.h>

#include "ast.h"
#include "ir.h"

extern AstNode *parse(char *filename);

int main(int argc, char *argv[])
{
    AstNode *ast;
    if (argc == 2) ast = parse(argv[1]);
    else return 0;
    print_ast(ast);
    printf("\nAST printing done.\n");
    delete_ast(ast);
    //ast_to_ir(ast);
    //ir_print();
}
