#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include "ast.h"

void code_gen(AstNode *ast, const char *out_filename);

#endif
