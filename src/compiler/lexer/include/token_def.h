#pragma once

// ----------------------------------------
// Literal types (representing basic data types)
// ----------------------------------------

#define TK_INTEGER 258
#define TK_FLOAT 259
#define TK_STRING 260


// ----------------------------------------
// Keywords
// ----------------------------------------

#define TK_MAP 261  // The token representing the 'Map' structure
#define TK_ROOM 262  // The token representing the 'Room' definition

// ----------------------------------------
// Graph function-related tokens
// ----------------------------------------

#define TK_CONNECT 263  // The token representing the 'Connect' function
#define TK_DIRECTED_EDGE 264  // The token representing the directed edge ('->')
#define TK_BIDIRECTIONAL_EDGE 265  // The token representing the bidirectional edge ('<->')


#define TK_ORD_OPERATOR 266
#define TK_EQ_OPERATOR 267

/* ----------------------------------------
   General tokens
---------------------------------------- */

#define TK_IDENTIFIER 268
#define TK_COMMENT 269
#define TK_LEFT_BRACE 270
#define TK_RIGHT_BRACE 271
#define TK_SEMICOLON 272

// ----------------------------------------
// Invalid or unknown token types
// ----------------------------------------

#define TK_INVALID_INTEGER 273
#define TK_INVALID_FLOAT 274
#define TK_INVALID_STRING 275
#define TK_INVALID_IDENTIFIER 276
#define TK_INVALID 277
