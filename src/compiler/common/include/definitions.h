#pragma once
#include <stddef.h>

#define MAX_INPUT_SIZE 32 
// Input to set max token length preventing user to overload compiler fx
// identifier with length of 2k

typedef enum {
  T_INTEGER,              // int
  T_FLOAT,                // float
  T_MAP,                  // Map
  T_ROOM,                 // Room
  T_CONNECT,              // Connect
  T_LBRACE,               // [
  T_RBRACE,               // ]
  T_LPAREN,               // (
  T_RPAREN,               // )
  T_DIRECTED_EDGE,        // ->
  T_BIDIRECTIONAL_EDGE,   // <->
  T_IDENTIFIER,           // Variable/function name
  T_SEMICOLON,            // ;
  T_EOF,                  // End Of File 
  T_MAP_CONSTR_ROOMS,     // maxRooms
  T_MAP_CONSTR_CONNECT,   // maxConnecPerRoom 
  T_EQUAL,                // =
} TokenDef;

typedef enum {
  AT_MAP = T_MAP,                                   // Map
  AT_ROOM = T_ROOM,                                 // Room
  AT_CONNECT = T_CONNECT,                           // Connect
  AT_DIRECTED_EDGE = T_DIRECTED_EDGE,               // ->
  AT_BIDIRECTIONAL_EDGE = T_BIDIRECTIONAL_EDGE,     // <->
  AT_IDENTIFIER = T_IDENTIFIER,                     // Variable/function name
  AT_MAP_CONSTR_ROOMS = T_MAP_CONSTR_ROOMS,                              // maxROOMS abstract token definition 
  AT_MAP_CONSTR_CONNECT = T_MAP_CONSTR_CONNECT,                            // maxConnectPerRoom abstract token definition 
  AT_CONSTR_VALUE,

} AbstractTokenDef;

