#pragma once

#include <stddef.h>
#include "definitions.h"

extern bool isTestMode;
void setTestMode(bool mode);
int reportLexerError(const char *input,  CLoc loc);
int reportParserError(const char *expected_token, const char *obtained_token, CLoc loc); 
