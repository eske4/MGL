#pragma once


#include <stddef.h>

extern bool isTestMode;
void setTestMode(bool mode);
int reportLexerError(const char *input,  size_t l, size_t c);
int reportParserError(const char *expected_token, const char *obtained_token, size_t line, size_t column); 
