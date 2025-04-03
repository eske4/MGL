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

typedef struct {
    const char* name;
    const TokenDef token;
} keyword;

typedef struct {
    const char symbol;
    const TokenDef token;
} delimiter;


static const keyword keyword_map[] = {
    {"Map", T_MAP},
    {"Room", T_ROOM},
    {"Connect", T_CONNECT},
    {"->", T_DIRECTED_EDGE},
    {"<->", T_BIDIRECTIONAL_EDGE},
    {"(", T_LPAREN},
    {")", T_RPAREN},
    {"{", T_LBRACE},
    {"}", T_RBRACE},
    {";", T_SEMICOLON},
    // Add more keywords here if needed
};

int scan(Token *t);
