#include "parser.h"
#include "astree.h"
#include "compiler_state.h"
#include "definitions.h"
#include "error_handler.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *mapExpr(Token *currentToken);
ASTNode *parseExpr(Token *currentToken);
ASTNode *roomExpr(Token *currentToken);
ASTNode *connectExpr(Token *currentToken);
int consumeToken(Token *currentToken, TokenDef expectedTokenType);
void reportParserError(const char *token);

ASTree parse(Token *currentToken) {
  ASTree tree = ASTInit(); // Initialize AST
  ASTNode *mapLeaf = mapExpr(currentToken);
  tree->head = mapLeaf;
  return tree; // Return the constructed AST
}

ASTNode *mapExpr(Token *currentToken) {

  if (!consumeToken(currentToken, T_MAP)) {
    reportParserError("Map");
    return NULL;
  }

  ASTNode *mapNameNode = ASTCreateIdentifier(currentToken->value.stringValue);

  if (!consumeToken(currentToken, T_IDENTIFIER)) {
    printf("Error: No identifier");
    reportParserError("identifier");
    return NULL;
  }

  if (!consumeToken(currentToken, T_LBRACKET)) {
    reportParserError("{");
    return NULL;
  }

  ASTNode *firstExprNode = parseExpr(currentToken);

  if (!consumeToken(currentToken, T_RBRACKET)) {
    reportParserError("}");
    return NULL;
  }

  return ASTCreateMap(mapNameNode, firstExprNode);
}

ASTNode *parseExpr(Token *currentToken) {
  if (currentToken->token == T_RBRACKET || currentToken->token == T_EOF) {
    return NULL;
  }

  ASTNode *node = NULL;

  switch (currentToken->token) {
  case T_ROOM:
    node = roomExpr(currentToken);
    break;
  case T_CONNECT:
    node = connectExpr(currentToken); // parse connect expressions
    break;
  default:
    reportParserError("Unexpected");
  }

  // After parsing the current node, check the next token to continue the
  // expression chain.
  ASTNode *nextNode = parseExpr(currentToken);

  return ASTCreateExpressionChain(
      node, nextNode); // Return the current node with its linked right child
}

ASTNode *roomExpr(Token *currentToken) {
  ASTNode *left = NULL;
  ASTNode *right = NULL;

  if (!consumeToken(currentToken, T_ROOM)) {
    reportParserError("Room");
    return NULL;
  }

  left = ASTCreateIdentifier(currentToken->value.stringValue);

  if (!consumeToken(currentToken, T_IDENTIFIER)) {
    reportParserError("identifier");
    return NULL;
  }

  if (!consumeToken(currentToken, T_SEMICOLON)) {
    reportParserError(";");
    return NULL;
  }

  return ASTCreateRoom(left); // Create and return a room AST node
}

ASTNode *connectExpr(Token *currentToken) {
  if (!consumeToken(currentToken, T_CONNECT) ||
      !consumeToken(currentToken, T_LPAREN)) {
    reportParserError("(");
    return NULL;
  }

  ASTNode *left = ASTCreateIdentifier(currentToken->value.stringValue);
  if (!consumeToken(currentToken, T_IDENTIFIER)) {
    reportParserError("Identifier");
    return NULL;
  }

  Type edgeType = (currentToken->token == T_DIRECTED_EDGE) ? TYPE_DIRECTED_EDGE
                  : (currentToken->token == TYPE_BIDIRECTIONAL_EDGE)
                      ? TYPE_BIDIRECTIONAL_EDGE
                      : -1;

  if (edgeType == -1) {
    reportParserError("-> or <->");
    return NULL;
  }

  scan(currentToken); // Consume edge token

  ASTNode *right = ASTCreateIdentifier(currentToken->value.stringValue);
  if (!consumeToken(currentToken, T_IDENTIFIER)) {
    reportParserError("identifier");
    return NULL;
  }

  if (!consumeToken(currentToken, T_RPAREN) ||
      !consumeToken(currentToken, T_SEMICOLON)) {
    reportParserError(") followed by ;");
    return NULL;
  }

  return (edgeType == TYPE_DIRECTED_EDGE)
             ? ASTCreateDirectedEdge(left, right)
             : ASTCreateBidirectionalEdge(left, right);
}

int consumeToken(Token *currentToken, TokenDef expectedTokenType) {
  if (currentToken == NULL) {
    reportError("Token is empty", "", cs.line, cs.column);
  }
  if (currentToken->token == expectedTokenType) {
    // Pass currentToken to scan, no need to use '&'
    scan(currentToken); // Update the current token using the lexer
    return 1;           // Successfully consumed the token
  }
  return 0; // Failed to consume token
}

void reportParserError(const char *token) {
  reportError("Parser error: Bad token expected: ", token, cs.line, cs.column);
}
