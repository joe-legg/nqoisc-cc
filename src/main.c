#include "ast.h"

extern AstNode *parse(char *filename);

int main(int argc, char *argv[])
{
    AstNode *ast;
    if (argc == 2)
        ast = parse(argv[1]);
    print_ast(ast);
}
