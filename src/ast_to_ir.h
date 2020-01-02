#ifndef AST_TO_IR_H
#define AST_TO_IR_H

#include "ir.h"
#include "ast.h"

IrInstr *ast_to_ir(AstNode *ast);

#endif
