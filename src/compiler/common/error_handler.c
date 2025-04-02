#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isTestMode = false; // Global test mode flag

void setTestMode(bool mode)
{
    isTestMode = mode;
}

// Helper function to print error messages
static void printError(const char* type, const char* message, size_t line, size_t column)
{
    if (!message)
    {
        fprintf(stderr, "%s error: <unknown error> at line %zu, column %zu\n", type, line, column);
    }
    else
    {
        fprintf(stderr, "%s error: %s at line %zu, column %zu\n", type, message, line, column);
    }

    if (!isTestMode)
    {
        exit(EXIT_FAILURE);
    }
}

int reportLexerError(const char* input, size_t line, size_t column)
{
    printError("Lexer", input, line, column);

    return -1; // Return error code instead of forcing termination
}

int reportParserError(const char* expected_token, const char* obtained_token, size_t line, size_t column)
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

    printError("Parser", buffer, line, column);

    return -1;
}
