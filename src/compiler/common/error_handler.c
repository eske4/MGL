#include "error_handler.h"
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isTestMode = false; // Global test mode flag

void setTestMode(bool mode)
{
    isTestMode = mode;
}

// Helper function to print error messages
static void printError(const char* type, const char* message, CLoc loc)
{
    if (!message)
    {
        fprintf(stderr, "%s error: <unknown error> at line %d, column %d\n", type, loc.line, loc.column);
    }
    else
    {
        fprintf(stderr, "%s error: %s at line %d, column %d\n", type, message, loc.line, loc.column);
    }

    if (!isTestMode)
    {
        exit(EXIT_FAILURE);
    }
}

int reportLexerError(const char* input, CLoc loc)
{
    printError("Lexer", input, loc);

    return -1; // Return error code instead of forcing termination
}

int reportParserError(const char* expected_token, const char* obtained_token, CLoc loc)
{
    char buffer[256]; // Buffer to hold formatted error message

    if (!expected_token || !obtained_token)
    {
        snprintf(buffer, sizeof(buffer), "NULL token provided");
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "expected token: %s, but got: %s", expected_token, obtained_token);
    }

    printError("Parser", buffer, loc);

    return -1;
}


