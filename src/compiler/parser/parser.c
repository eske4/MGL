#include "parser.h"
#include "astree.h"
#include "compiler_state.h"
#include "definitions.h"
#include "error_handler.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* mapExpr(ASTree tree, Token* currentToken);
void buildMapExpr(ASTNode* mapNode, Token* currentToken);
void roomExpr(ASTNode* mapNode, Token* currentToken);
void connectExpr(ASTNode* mapNode, Token* currentToken);
int consumeToken(Token* currentToken, TokenDef expectedTokenType);
void errorHelper(const char* expected_token, const Token* obtained_token);

ASTree parse(Token* currentToken)
{
    ASTree tree      = ASTInit(); // Initialize AST
    ASTNode* mapNode = mapExpr(tree, currentToken);
    tree->head       = mapNode;
    return tree; // Return the constructed AST
}

ASTNode* mapExpr(ASTree tree, Token* currentToken)
{
    if (!consumeToken(currentToken, T_MAP))
        errorHelper("Map", currentToken);

    const char* id = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        errorHelper("identifier", currentToken);

    ASTNode* mapNode = ASTCreateMap(tree, id);

    if (!consumeToken(currentToken, T_LBRACE))
        errorHelper("{", currentToken);

    buildMapExpr(mapNode, currentToken);

    if (!consumeToken(currentToken, T_RBRACE))
        errorHelper("}", currentToken);

    return mapNode;
}

void buildMapExpr(ASTNode* mapNode, Token* currentToken)
{
    if (currentToken->token == T_RBRACE || currentToken->token == T_EOF)
    {
        return;
    }

    switch (currentToken->token)
    {
        case T_ROOM: roomExpr(mapNode, currentToken); break;
        case T_CONNECT: connectExpr(mapNode, currentToken); break;
        default: errorHelper("Connect | Room", currentToken);
    }

    buildMapExpr(mapNode, currentToken);
}

void roomExpr(ASTNode* mapNode, Token* currentToken)
{
    if (!consumeToken(currentToken, T_ROOM)) // Consumes "room"
        errorHelper("Room", currentToken);

    const char* id = strdup(currentToken->value.stringValue); // Identifier expected

    if (!consumeToken(currentToken, T_IDENTIFIER)) // Should consume the identifier
        errorHelper("identifier", currentToken);

    if (!consumeToken(currentToken, T_SEMICOLON)) // Consumes ";"
        errorHelper(";", currentToken);

    ASTCreateRoom(mapNode, id); // Create and return a room AST node
}

void connectExpr(ASTNode* mapNode, Token* currentToken)
{
    if (!consumeToken(currentToken, T_CONNECT) || !consumeToken(currentToken, T_LPAREN))
        errorHelper("(", currentToken);

    const char* id = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        errorHelper("Identifier", currentToken);

    TokenDef op = currentToken->token;

    if (op != T_DIRECTED_EDGE && op != T_BIDIRECTIONAL_EDGE)
        errorHelper("-> | <->", currentToken);

    scan(currentToken); // Consume edge token

    const char* id2 = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        errorHelper("identifier", currentToken);

    if (!consumeToken(currentToken, T_RPAREN) || !consumeToken(currentToken, T_SEMICOLON))
        errorHelper(") + ;", currentToken);

    ASTCreateConnect(mapNode, id, op, id2);
}

int consumeToken(Token* currentToken, TokenDef expectedTokenType)
{
    if (currentToken == NULL)
    {
        fprintf(stderr, "Expected a token but got NULL");
    }

    if (currentToken->token == expectedTokenType)
    {
        // Pass currentToken to scan, no need to use '&'
        scan(currentToken); // Update the current token using the lexer
        return 1;           // Successfully consumed the token
    }

    return 0; // Failed to consume token
}

void errorHelper(const char* expected_token, const Token* obtained_token)
{
    const char* o_input = obtained_token->value.stringValue;

    reportParserError(expected_token, o_input, cs.line, cs.column);
}
