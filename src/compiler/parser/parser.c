#include "parser.h"
#include "astree.h"
#include "definitions.h"
#include "error_handler.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>

ASTNode* mapExpr(ASTree tree, Token* currentToken);
void mapConstrExpr(ASTNode* constrNode, Token* currentToken);
void buildMapExpr(ASTNode* mapNode, Token* currentToken);
void roomExpr(ASTNode* mapNode, Token* currentToken);
void connectExpr(ASTNode* mapNode, Token* currentToken);
int consumeToken(Token* currentToken, TokenDef expectedTokenType);
void reportParserError(const char* expected_token, const Token* obtained_token);

// function to parse single token 
ASTree parse(Token* currentToken)
{
    ASTree tree      = ASTInit(); // Initialize AST
    ASTNode* mapNode = mapExpr(tree, currentToken);
    tree->head       = mapNode;
    return tree; // Return the constructed AST
}


ASTNode* mapExpr(ASTree tree, Token* currentToken)
{
    int mapPos = currentToken->pos;
    if (!consumeToken(currentToken, T_MAP))
        reportParserError("Map", currentToken);

    const char* id = strdup(currentToken->value);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        reportParserError("identifier", currentToken);

    if (consumeToken(currentToken, T_LPAREN)) {

        ASTNode* mapConstrNode = ASTCreateMapConstr(tree, id, mapPos);

        mapConstrExpr(mapConstrNode, currentToken);

        if (!consumeToken(currentToken, T_RPAREN))
        reportParserError(")", currentToken);
    }

    ASTNode* mapNode = ASTCreateMap(tree, id, mapPos);

    if (!consumeToken(currentToken, T_LBRACE))
        reportParserError("{", currentToken);

    buildMapExpr(mapNode, currentToken);

    if (!consumeToken(currentToken, T_RBRACE))
        reportParserError("}", currentToken);

    return mapNode;
}

void mapConstrExpr(ASTNode* constrNode, Token* currentToken){
    if (currentToken->token == T_RPAREN) 
    {
        return;
    }

    if(consumeToken(currentToken, T_MAP_CONSTR_ROOMS)){ 
        if(!consumeToken(currentToken, T_EQUAL))
            reportParserError("=", currentToken);
        if(!consumeToken(currentToken, T_INTEGER))
            reportParserError("an Integer", currentToken);
        if(!consumeToken(currentToken, T_SEMICOLON))
            reportParserError(";", currentToken);
    } else if(consumeToken(currentToken, T_MAP_CONSTR_CONNECT)){
        if(!consumeToken(currentToken, T_EQUAL))
            reportParserError("=", currentToken);
        if(!consumeToken(currentToken, T_INTEGER))
            reportParserError("an Integer", currentToken);
        if(!consumeToken(currentToken, T_SEMICOLON))
            reportParserError(";", currentToken);
    } else {
        reportParserError("maxRooms | maxConnectPerRoom", currentToken);
    }; 

    mapConstrExpr(constrNode, currentToken);

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
        default: reportParserError("Connect | Room", currentToken);
    }

    buildMapExpr(mapNode, currentToken);
}



void roomExpr(ASTNode* mapNode, Token* currentToken)
{
    int roomPos = currentToken->pos;
    if (!consumeToken(currentToken, T_ROOM)) // Consumes "room"
        reportParserError("Room", currentToken);

    const char* id = strdup(currentToken->value); // Identifier expected

    if (!consumeToken(currentToken, T_IDENTIFIER)) // Should consume the identifier
        reportParserError("identifier", currentToken);

    if (!consumeToken(currentToken, T_SEMICOLON)) // Consumes ";"
        reportParserError(";", currentToken);

    ASTCreateRoom(mapNode, id, roomPos); // Create and return a room AST node
}

void connectExpr(ASTNode* mapNode, Token* currentToken)
{
    int connectPos = currentToken->pos;
    if (!consumeToken(currentToken, T_CONNECT) || !consumeToken(currentToken, T_LPAREN))
        reportParserError("(", currentToken);

    const char* id = strdup(currentToken->value);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        reportParserError("Identifier", currentToken);

    TokenDef op = currentToken->token;

    if (op != T_DIRECTED_EDGE && op != T_BIDIRECTIONAL_EDGE)
        reportParserError("-> | <->", currentToken);

    scan(currentToken); // Consume edge token

    const char* id2 = strdup(currentToken->value);

    if (!consumeToken(currentToken, T_IDENTIFIER))
        reportParserError("identifier", currentToken);

    if (!consumeToken(currentToken, T_RPAREN) || !consumeToken(currentToken, T_SEMICOLON))
        reportParserError(") + ;", currentToken);

    ASTCreateConnect(mapNode, id, op, id2, connectPos);
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

void reportParserError(const char* expected_token, const Token* obtained_token)
{
    const char* o_input = obtained_token->value;
    const char *msg[] = {"Expected ", expected_token, " but got: ", o_input};
    reportError(ERR_PARSER, obtained_token->pos, msg, 4);
}
