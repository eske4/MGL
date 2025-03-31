#pragma once

#include "astree.h"
#include "lexer.h"

// Parser functions accepting the current token
ASTree parse(Token *currentToken); // Parse a primary expression
