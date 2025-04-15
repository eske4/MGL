#include "error_handler.h"
#include "compiler_state.h"
#include "safe_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int isTestMode = false; // Global test mode flag

void setTestMode(int mode)
{
    isTestMode = mode;
}

const char *const ERROR_LABELS[ERR_COUNT] = {
    [ERR_UNKNOWN] = "Unknown error",
    [ERR_LEXER]   = "Lexical error: ",
    [ERR_PARSER]  = "Syntax error: ",
    [ERR_SEMANTIC] = "Semantic error: ",
    [ERR_TYPE]     = "Type error: "
};

int reportError(ErrorCode code, int pos, const char *strings[], size_t msgc) {
    if(isTestMode) return code;

    char msg[MAX_ERROR_LENGTH] = {0};
    int status = safe_multi_strcat(msg, strings, msgc, MAX_ERROR_LENGTH);
    CLoc loc = findLoc(pos);
    if(status == -1){
        fprintf(stderr, "Failed to build the error message");
    }
   
    fprintf(stderr, "%d:%d: %s %s\n", loc.line, loc.column, ERROR_LABELS[code], msg);

    exit(1);
}
