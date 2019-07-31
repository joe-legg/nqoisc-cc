%{
#include "src/parser.h"
%}

%option nounput
%option noinput
%option noyywrap

%option yylineno

%%

"int"      return TOK_KWD_INT;
"return"   return TOK_KWD_RETURN;
"const"    return TOK_KWD_CONST;
"restrict" return TOK_KWD_RESTRICT;
"volatile" return TOK_KWD_VOLATILE;

"("   return TOK_LPAREN;
")"   return TOK_RPAREN;
"{"   return TOK_LBRACE;
"}"   return TOK_RBRACE;
","   return TOK_COMMA;
";"   return TOK_SEMICOLON;
"..." return TOK_ELLIPSIS;

[0-9]+                 return TOK_DECIMAL;
[_a-zA-Z][_a-zA-Z0-9]* return TOK_IDENT;
\'(\\.|[^"\\\n])\'     return TOK_CHAR;
\'(\\.|[^"\\\n])*\'    return TOK_MULTI_CHAR;

\/\*(.*\n)*.*\*\/ {}
\/\/.* {}
" "|\n|\t|\r {}

. return 999;

%%
