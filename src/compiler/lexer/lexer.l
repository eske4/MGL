%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error_handler.h"
#include "lexer.h"
#include "parser.tab.h"

int process_token(int type);
void assign_yylval(const char *lexeme, int type);

%}

%option yylineno
%option noyywrap

INVALID             . 

INTEGER             (0|[1-9][0-9]*)
FLOAT               {INTEGER}"."([0-9]+)
STRING              \"([^\"\n]*)\"
WHITESPACE          [ \t\n\r]+
MAP                 Map
ROOM                Room
CONNECT             Connect
ORD                 "<="|">="|"<"|">"|"AND"|"OR"
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

{IDENTIFIER} { return process_token(TK_IDENTIFIER);}

-->       {}
{INVALID} { reportError("Bad token!", yylineno, 0); }

%%

int process_token(int type) {
    assign_yylval(yytext, type);
    return type;
}

void assign_yylval(const char *lexeme, int type) {
    switch (type) {
        case TK_INTEGER: yylval.num = atoi(lexeme); break;
        case TK_FLOAT: yylval.flt = atof(lexeme); break;
        default: 
            if(strlen(lexeme) > 21){
                reportError("Oversized identifier", yylineno, 0);
            } 

            strncpy(yylval.str, lexeme, 21 - 1);
            yylval.str[21 - 1] = '\0'; 
            break;
    }
}



int tokenize(const char* input) {
    yy_scan_string(input);  // Scan the string directly
    return yylex();  // Return the first token
}
