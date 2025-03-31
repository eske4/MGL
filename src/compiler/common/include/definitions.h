#pragma once
#include <stddef.h>

#define MAX_INPUT_SIZE                                                         \
  30 // Input to set max token length preventing user to overload compiler fx
     // identifier with length of 2k

typedef enum {
  T_INTEGER,            // int
  T_FLOAT,              // float
  T_MAP,                // Map
  T_ROOM,               // Room
  T_CONNECT,            // Connect
  T_LBRACKET,           // [
  T_RBRACKET,           // ]
  T_LPAREN,             // (
  T_RPAREN,             // )
  T_DIRECTED_EDGE,      // ->
  T_BIDIRECTIONAL_EDGE, // <->
  T_IDENTIFIER,         // Variable/function name
  T_SEMICOLON,          // ;
  T_EOF
} TokenDef;

typedef enum {
  TYPE_IDENTIFIER,
  TYPE_MAP,
  TYPE_ROOM,
  TYPE_CONNECT,
  TYPE_DIRECTED_EDGE,
  TYPE_BIDIRECTIONAL_EDGE,
  TYPE_EXPRESSION_CHAIN, // <-- Enabling chaining Room and Connect inside Map
                         // can
                         // be used for more later
  TYPE_INTEGER_LITERAL,
  TYPE_STRING_LITERAL,
  TYPE_FLOAT_LITERAL
} Type;
