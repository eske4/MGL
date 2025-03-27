%{
#include "error_handler.h"
#include <stdio.h>
#include "parser.h"
#include "lexer.h"

#ifdef __cplusplus
extern "C" {
#endif

int yylex();
int yyerror(char *s);
extern void yy_scan_string(const char* input);
extern char* yytext;

#ifdef __cplusplus
}
#endif

%}

%token <num> TK_INTEGER
%token <flt> TK_FLOAT
%token <str> TK_STRING
%token <str> TK_MAP TK_ROOM TK_CONNECT
%token <str> TK_DIRECTED_EDGE TK_BIDIRECTIONAL_EDGE
%token <str> TK_ORD_OPERATOR TK_EQ_OPERATOR
%token <str> TK_IDENTIFIER
%token <str> TK_COMMENT
%token <str> TK_LEFT_BRACE TK_RIGHT_BRACE TK_LEFT_PAREN TK_RIGHT_PAREN
%token <str> TK_SEMICOLON
%token <str> TK_INVALID

%union {
    int num;    // Integer type
    float flt;  // Float type
    char str[21];  // String type
}


%%

prog:
    map_decl
    |
    map_imp
;

map_decl:
    TK_MAP
    TK_IDENTIFIER
    TK_LEFT_BRACE
    stmts
    TK_RIGHT_BRACE
;

map_imp:
    TK_MAP
    TK_IDENTIFIER
    TK_SEMICOLON
    stmts
;

stmts:
    stmts
    stmt
    |
    /* Empty */
;

stmt:
    room
    | connect_func

room:
    TK_ROOM
    TK_IDENTIFIER
    TK_SEMICOLON
;

connect_func:
    TK_CONNECT 
    TK_LEFT_PAREN 
    TK_IDENTIFIER 
    TK_DIRECTED_EDGE 
    TK_IDENTIFIER 
    TK_RIGHT_PAREN 
    TK_SEMICOLON
;
%%

int yyerror(char *s)
{
	printf("Syntax Error on line %s\n", s);
	return 0;
}


int parse(const char* input) {
    yy_scan_string(input);  // Set input to be processed by the lexer
    return yyparse();  // Return result from the parser (0 on success, non-zero on error)
}

