%{ 
/* C stuff - headers, declarations, variables, ect. */

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

/* Define any additional helper functions or variables here. */
%}

/* Rules Section */ 
%%

"#".*                           { /* Skip comments starting with '#' */ }

"Map"                           { return MAP; }
"Room"                          { return ROOM; }
"Connect"                       { return CONNECT; }
"List<"[a-zA-Z0-9]+">"          { return LIST; }
"String"                        { return STRING_TYPE; }
"Bool"                          { return BOOL_TYPE; }
"Int"                           { return INT_TYPE; }

[a-z][a-zA-Z0-9]*               { return IDENTIFIER; }

"->"                           { return ARROW; }
"<->"                          { return BICONNECT; }
"<-"                           { return LARROW; }

"=="                           { return EQ; }
"!="                           { return NE; }
"&&"                           { return AND; }
"||"                           { return OR; }

">="                           { return GE; }
"<="                           { return LE; }
">"                            { return GT; }
"<"                            { return LT; }

"+"                            { return PLUS; }
"-"                            { return MINUS; }
"*"                            { return MULT; }
"/"                            { return DIV; }

0|([1-9][0-9]*)                { yylval.intVal = atoi(yytext); return INT_LITERAL; }

\"([^\\"]*)\"                  { yylval.stringVal = strdup(yytext); return STRING_LITERAL; }


";"                            { return SEMICOLON; }
"{"                            { return LBRACE; }
"}"                            { return RBRACE; }
"("                            { return LPAREN; }
")"                            { return RPAREN; }

[ \t\n]+                       { /* Skip whitespace */ }

.                              { printf("Unrecognized token: %s\n", yytext); }

%% 

/* User Code Section */