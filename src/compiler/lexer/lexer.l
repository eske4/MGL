%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lexer.h" // Contains token data structure and function definitions for the Lexer

int process_token(int type);

#ifdef __BISON__ // To include needed headers and functions, when compiled with Bison
#include "parser.h"
void assign_yylval(const char *lexeme, int type) {
    switch (type) {
        case TK_INTEGER: yylval.num = atoi(lexeme); break;
        case TK_FLOAT: yylval.flt = atof(lexeme); break;
        default: yylval.str = strdup(lexeme); break;
    }
}
#else
#include "token_def.h" // Header file containing the token definitions, when not compiled with Bison
#endif
%}

INVALID_INTEGER     0[0-9]+([0-9]*)
INVALID_FLOAT       0[0-9]+\.[0-9]+|[0-9]*\.[0-9]*\.|[0-9]+\.[0-9]+\.[0-9]+ 
INVALID_STRING      \"(\\(.|\n)|[^\\"\n])*
INVALID_IDENTIFIER  [0-9][a-zA-Z0-9_]*

INVALID             . 

INTEGER             (0|[1-9][0-9]*)
FLOAT               {INTEGER}"."([0-9]+)
STRING              \"(\\.|[^"\\])*\"
WHITESPACE          [ \t\n\r]+
MAP                 Map
ROOM                Room
CONNECT             Connect
ORD                 "=="|"<="|">="|"<"|">"|"AND"|"OR"
EQ                 "=="|"!="
DIRECTED_EDGE       "->"
BIDIRECTIONAL_EDGE  "<->"
COMMENT             \#.*
NEWLINE             \n
SEMICOLON           \;
LEFT_BRACE          \{
RIGHT_BRACE         \}
LEFT_PAREN          \(
RIGHT_PAREN         \)
IDENTIFIER          [a-zA-Z_][a-zA-Z0-9_]*

%%

{NEWLINE} { }
{WHITESPACE} { }
{COMMENT} { }

{INVALID_FLOAT} { return process_token(TK_INVALID_FLOAT);}
{INVALID_INTEGER} { return process_token(TK_INVALID_INTEGER);}
{INVALID_STRING} { return process_token(TK_INVALID_STRING);}

{INTEGER} { return process_token(TK_INTEGER);}
{FLOAT} { return process_token(TK_FLOAT);}
{STRING} { return process_token(TK_STRING);}

{MAP} { return process_token(TK_MAP);}
{ROOM} { return process_token(TK_ROOM);}
{CONNECT} { return process_token(TK_CONNECT);}

{ORD} { return process_token(TK_ORD_OPERATOR);}
{DIRECTED_EDGE} { return process_token(TK_DIRECTED_EDGE);}
{BIDIRECTIONAL_EDGE} { return process_token(TK_BIDIRECTIONAL_EDGE);}
{EQ} { return process_token(TK_EQ_OPERATOR);}

{LEFT_BRACE} { return process_token(TK_LEFT_BRACE);}
{RIGHT_BRACE} { return process_token(TK_RIGHT_BRACE);}
{LEFT_PAREN} { return process_token(TK_LEFT_PAREN);}
{RIGHT_PAREN} { return process_token(TK_RIGHT_PAREN);}
{SEMICOLON} { return process_token(TK_SEMICOLON);}

{INVALID_IDENTIFIER} { return process_token(TK_INVALID_IDENTIFIER);}
{IDENTIFIER} { return process_token(TK_IDENTIFIER);}

{INVALID} { return process_token(TK_INVALID); }

%%

int process_token(int type) {
#ifdef __BISON__
    assign_yylval(yytext, type);
#endif
    return type;
}

int tokenize(const char* input) {
    yy_scan_string(input);  // Scan the string directly
    return yylex();  // Return the first token
}

int yywrap() { return 1; }
