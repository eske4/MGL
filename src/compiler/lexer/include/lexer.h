#pragma once

#include "definitions.h"

typedef struct Token {
  TokenDef token;
  union {
    long intValue; // 32bit int
    float floatValue;
    char stringValue[MAX_INPUT_SIZE]; // Limit token size, to prevent memory
                                      // overload
  } value;
} Token;

int scan(Token *t);
