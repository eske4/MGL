#pragma once

#include "definitions.h"

typedef struct Token {
  TokenDef token;
  int pos;
  char value[MAX_INPUT_SIZE]; // Limit token size, to prevent memory
} Token;

typedef struct {
    const char* name;
    const TokenDef token;
} Map;

// REMEMBER TO UPDATE IF ADDING TO KEYWORD_MAP AND DELIMITER_MAP
const static int keyword_map_size = 7;
const static int delimiter_map_size = 6;

static const Map keyword_map[] = {
    {"Map", T_MAP},
    {"Room", T_ROOM},
    {"Connect", T_CONNECT},
    {"->", T_DIRECTED_EDGE},
    {"<->", T_BIDIRECTIONAL_EDGE},
    {"maxRooms", T_MAP_CONSTR_ROOMS}, 
    {"maxConnectPerRoom", T_MAP_CONSTR_CONNECT}, 
    // Add more keywords here if needed
};

static const Map delimiter_map[] = {
    {"(", T_LPAREN},
    {")", T_RPAREN},
    {"{", T_LBRACE},
    {"}", T_RBRACE},
    {";", T_SEMICOLON},
    {"=", T_EQUAL}, 
    // Add more delimiter
};

int scan(Token *t);
