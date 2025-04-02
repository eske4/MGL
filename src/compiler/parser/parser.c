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
void reportParserError(const char* token);

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
    {
        reportParserError("Map");
        return NULL;
    }

    const char* id = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
    {
        printf("Error: No identifier");
        reportParserError("identifier");
        return NULL;
    }

    ASTNode* mapNode = ASTCreateMap(tree, id);

    if (!consumeToken(currentToken, T_LBRACE))
    {
        reportParserError("{");
        return NULL;
    }

    buildMapExpr(mapNode, currentToken);

    if (!consumeToken(currentToken, T_RBRACE))
    {
        reportParserError("}");
        return NULL;
    }

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
        default: reportParserError("Unexpected");
    }

    buildMapExpr(mapNode, currentToken);
}

void roomExpr(ASTNode* mapNode, Token* currentToken)
{
    if (!consumeToken(currentToken, T_ROOM)) // Consumes "room"
    {
        reportParserError("Room");
        return;
    }

    const char* id = strdup(currentToken->value.stringValue); // Identifier expected
    printf("%s", id);

    if (!consumeToken(currentToken, T_IDENTIFIER)) // Should consume the identifier
    {
        reportParserError("identifier was in room");
        return;
    }

    if (!consumeToken(currentToken, T_SEMICOLON)) // Consumes ";"
    {
        reportParserError(";");
        return;
    }

    ASTCreateRoom(mapNode, id); // Create and return a room AST node
}

void connectExpr(ASTNode* mapNode, Token* currentToken)
{
    if (!consumeToken(currentToken, T_CONNECT) || !consumeToken(currentToken, T_LPAREN))
    {
        reportParserError("(");
        return;
    }

    const char* id = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
    {
        reportParserError("Identifier");
        return;
    }

    TokenDef op = currentToken->token;

    if (op != T_DIRECTED_EDGE && op != T_BIDIRECTIONAL_EDGE)
    {
        reportParserError("-> or <->");
        return;
    }

    scan(currentToken); // Consume edge token

    const char* id2 = strdup(currentToken->value.stringValue);

    if (!consumeToken(currentToken, T_IDENTIFIER))
    {
        reportParserError("identifier was in connect");
        return;
    }

    if (!consumeToken(currentToken, T_RPAREN) || !consumeToken(currentToken, T_SEMICOLON))
    {
        reportParserError(") followed by ;");
        return;
    }

    ASTCreateConnect(mapNode, id, op, id2);
}

int consumeToken(Token* currentToken, TokenDef expectedTokenType)
{
    if (currentToken == NULL)
    {
        reportLexerError("Token is empty", "", cs.line, cs.column);
    }

    if (currentToken->token == expectedTokenType)
    {
        // Pass currentToken to scan, no need to use '&'
        scan(currentToken); // Update the current token using the lexer
        return 1;           // Successfully consumed the token
    }

    return 0; // Failed to consume token
}

void reportParserError(const char* token)
{
    reportLexerError("Parser error: Bad token expected: ", token, cs.line, cs.column);
}
