%{
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
%token <str> TK_MAP
%token <str> TK_ROOM
%token <str> TK_CONNECT
%token <str> TK_DIRECTED_EDGE
%token <str> TK_BIDIRECTIONAL_EDGE
%token <str> TK_ORD_OPERATOR
%token <str> TK_EQ_OPERATOR
%token <str> TK_IDENTIFIER
%token <str> TK_COMMENT
%token <str> TK_LEFT_BRACE
%token <str> TK_RIGHT_BRACE
%token <str> TK_LEFT_PAREN
%token <str> TK_RIGHT_PAREN
%token <str> TK_SEMICOLON
%token <str> TK_INVALID_INTEGER
%token <str> TK_INVALID_FLOAT
%token <str> TK_INVALID_STRING
%token <str> TK_INVALID_IDENTIFIER
%token <str> TK_INVALID

%union {
    int num;    // Integer type
    float flt;  // Float type
    char* str;  // String type
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
    rooms
    connect_funcs
    TK_RIGHT_BRACE
;

map_imp:
    TK_MAP
    TK_IDENTIFIER
    TK_SEMICOLON
    rooms
    connect_funcs
;

rooms:
    room
    rooms
    |
    /* Empty */
;

room:
    TK_ROOM
    TK_IDENTIFIER
    TK_SEMICOLON
;

connect_funcs:
    connect_func
    connect_funcs
    |
    /* Empty */
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
    // Prepare the lexer to scan the input string
    yy_scan_string(input);  // Set input to be processed by the lexer

    // Call the parser to process the input
    return yyparse();  // Return result from the parser (0 on success, non-zero on error)
}

