%{
#include "src/parser.h"
%}

%option nounput
%option noinput
%option noyywrap

%option yylineno

%%

"return"     return TOK_KWD_RETURN;
"const"      return TOK_KWD_CONST;
"restrict"   return TOK_KWD_RESTRICT;
"volatile"   return TOK_KWD_VOLATILE;
"void"       return TOK_KWD_VOID;
"char"       return TOK_KWD_CHAR;
"short"      return TOK_KWD_SHORT;
"long"       return TOK_KWD_LONG;
"int"        return TOK_KWD_INT;
"float"      return TOK_KWD_FLOAT;
"double"     return TOK_KWD_DOUBLE;
"signed"     return TOK_KWD_SIGNED;
"unsigned"   return TOK_KWD_UNSIGNED;
"inline"     return TOK_KWD_INLINE;
"typedef"    return TOK_KWD_TYPEDEF;
"extern"     return TOK_KWD_EXTERN;
"static"     return TOK_KWD_STATIC;
"auto"       return TOK_KWD_AUTO;
"register"   return TOK_KWD_REGISTER;
"struct"     return TOK_KWD_STRUCT;
"union"      return TOK_KWD_UNION;
"enum"       return TOK_KWD_ENUM;
"_Bool"      return TOK_KWD__BOOL;
"_Complex"   return TOK_KWD__COMPLEX;
"_Imaginary" return TOK_KWD__IMAGINARY;

"*"      return TOK_OP_ASTERISK;
"/"      return TOK_OP_DIVIDE;
"+"      return TOK_OP_PLUS;
"-"      return TOK_OP_MINUS;
"%"      return TOK_OP_MODULO;
"~"      return TOK_OP_TILDE;
"!"      return TOK_OP_NOT;
"="      return TOK_OP_EQUALS;
"|"      return TOK_OP_OR;
"&"      return TOK_OP_AND;
"^"      return TOK_OP_CARET;
"||"     return TOK_OP_LOGICAL_OR;
"&&"     return TOK_OP_LOGICAL_AND;
"=="     return TOK_OP_EQUALITY;
"!="     return TOK_OP_NOT_EQUAL;
"<"      return TOK_OP_LESS_THAN;
">"      return TOK_OP_MORE_THAN;
"<="     return TOK_OP_LESS_THAN_OR_EQUAL;
">="     return TOK_OP_MORE_THAN_OR_EQUAL;
"<<"     return TOK_OP_SHIFT_LEFT;
">>"     return TOK_OP_SHIFT_RIGHT;
"++"     return TOK_OP_PLUS_PLUS;
"--"     return TOK_OP_MINUS_MINUS;
"*="     return TOK_OP_TIMES_EQUALS;
"/="     return TOK_OP_DIVIDE_EQUALS;
"%="     return TOK_OP_MODULO_EQUALS;
"+="     return TOK_OP_PLUS_EQUALS;
"-="     return TOK_OP_MINUS_EQUALS;
"<<="    return TOK_OP_SHIFT_LEFT_EQUALS;
">>="    return TOK_OP_SHIFT_RIGHT_EQUALS;
"&="     return TOK_OP_AND_EQUALS;
"^="     return TOK_OP_XOR_EQUALS;
"|="     return TOK_OP_OR_EQUALS;
"->"     return TOK_OP_ARROW;
"sizeof" return TOK_OP_SIZEOF;


"("   return TOK_LPAREN;
")"   return TOK_RPAREN;
"{"   return TOK_LBRACE;
"}"   return TOK_RBRACE;
"["   return TOK_LSQUARE;
"]"   return TOK_RSQUARE;
"?"   return TOK_QUESTION_MARK;
","   return TOK_COMMA;
";"   return TOK_SEMICOLON;
":"   return TOK_COLON;
"."   return TOK_DOT;
"..." return TOK_ELLIPSIS;

[0-9]+                 return TOK_DECIMAL;
[_a-zA-Z][_a-zA-Z0-9]* return TOK_IDENT;
\"(\\.|[^"\\\n])*\"    return TOK_STR_LIT;
\'(\\.|[^"\\\n])\'     return TOK_CHAR;
\'(\\.|[^"\\\n])*\'    return TOK_MULTI_CHAR;

\/\*(.*\n)*.*\*\/ {}
\/\/.* {}
" "|\n|\t|\r {}

. return 999; // FIXME: when an unknown character is found the program crashes

%%

/*
static int check_ident_type()
{
    return 0;
}
*/
