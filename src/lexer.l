%{
#include "src/parser.h"
%}

%option nounput
%option noinput
%option noyywrap

%option yylineno

IS (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))

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
"_Bool"      return TOK_KWD_BOOL;
"_Complex"   return TOK_KWD_COMPLEX;
"_Imaginary" return TOK_KWD_IMAGINARY;

"inline"     return TOK_KWD_INLINE;
"typedef"    return TOK_KWD_TYPEDEF;
"extern"     return TOK_KWD_EXTERN;
"static"     return TOK_KWD_STATIC;
"auto"       return TOK_KWD_AUTO;
"register"   return TOK_KWD_REGISTER;
"struct"     return TOK_KWD_STRUCT;
"union"      return TOK_KWD_UNION;
"enum"       return TOK_KWD_ENUM;

"case"     return TOK_KWD_CASE;
"default"  return TOK_KWD_DEFAULT;
"if"       return TOK_KWD_IF;
"else"     return TOK_KWD_ELSE;
"switch"   return TOK_KWD_SWITCH;
"while"    return TOK_KWD_WHILE;
"do"       return TOK_KWD_DO;
"for"      return TOK_KWD_FOR;
"goto"     return TOK_KWD_GOTO;
"continue" return TOK_KWD_CONTINUE;
"break"    return TOK_KWD_BREAK;

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
">"      return TOK_OP_GREATER_THAN;
"<="     return TOK_OP_LESS_THAN_OR_EQUAL;
">="     return TOK_OP_GREATER_THAN_OR_EQUAL;
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

[1-9][0-9]*{IS}?       return TOK_DECIMAL_CONST;
0[0-7]*{IS}?           return TOK_OCTAL_CONST;
0[xX][0-9a-fA-F]+{IS}? return TOK_HEXADECIMAL_CONST;

[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?[flFL]?                   return TOK_DECIMAL_FLOAT;
[0-9]+([eE][+-]?[0-9]+)[flFL]?                            return TOK_DECIMAL_FLOAT;
(0[xX][a-fA-F0-9]*\.[a-fA-F0-9]+([Pp][+-]?[0-9]+)[flFL]?) return TOK_HEXADECIMAL_FLOAT;
(0[xX]*\.[a-fA-F0-9]+([Pp][+-]?[0-9]+)[flFL]?)            return TOK_HEXADECIMAL_FLOAT;
0[xX]+\.([Pp][+-]?[0-9]+)[flFL]?                          return TOK_HEXADECIMAL_FLOAT;

[_a-zA-Z][_a-zA-Z0-9]* return TOK_IDENT;
L?'(\\.|[^\\'\n])+'    return TOK_CHAR;
((u8|u|U|L)?\"([^"\\\n]|(\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+)))*\"[ \t\v\n\f]*)+ return TOK_STR_LIT;

\/\*(.*\n)*.*\*\/ {}
\/\/.* {}
" "|\n|\t|\r ;

. return 999; // FIXME: when an unknown character is found the program crashes

%%

static int check_ident_type()
{
    // TODO
    return 0;
}
