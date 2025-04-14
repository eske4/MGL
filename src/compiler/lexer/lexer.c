#include "lexer.h"
#include "compiler_state.h"
#include "definitions.h"
#include "error_handler.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set_token(Token* t, TokenDef type, const char* value)
{
    if (!t || !value || *value == '\0')
        return 0;

    t->token = type;
    t->pos = cs.pos;
    strlcpy(t->value, value, sizeof(t->value));
    return 1;
}

static void putback(int c)
{
    cs.putback = c;
}

static int next(void)
{
    if (cs.putback)
    {
        int c      = cs.putback;
        cs.putback = 0;
        return c;
    }

    int c = fgetc(cs.infile);
    if (c == '\n')
    {
        cs.line++;
        cs.column = 0;
    }
    else
    {
        cs.column++;
    }
    cs.pos++;

    return c;
}

static int skip(void)
{
    int c;
    while ((c = next()) && isspace(c))
        ;
    return c;
}

int match_keyword(Token* currentToken, const char* in)
{
    size_t keyword_count = sizeof(keyword_map) / sizeof(keyword_map[0]);
    for (int i = 0; i < keyword_count; i++)
    {
        if (strcmp(in, keyword_map[i].name) == 0)
        {
            set_token(currentToken, keyword_map[i].token, keyword_map[i].name);

            return 1;
        }
    }
    return 0;
}

int is_delimiter(int c) // Removed unused 'Token* t'
{
    switch (c)
    {
        case ';':
        case '(':
        case ')':
        case '{':
        case '}': return 1;
    }
    return 0;
}

int match_identifier(Token* t, int c)
{

    int i                       = 0;
    char buffer[MAX_INPUT_SIZE] = {0};
    int isIdentifier            = 1;
    int isDelimiter             = 0;
    int isKeyword               = 0;

    while (!strchr(" \t\n\r\f", c) && i < MAX_INPUT_SIZE - 1)
    {
        buffer[i++] = c;

        if (is_delimiter(c))
        {
            isDelimiter = 1;
            break;
        }

        c = next();

        if (is_delimiter(c))
        {
            putback(c);
            isDelimiter = 1;
            break;
        }

        if (!(isalnum(c) || c == '_') && !isspace(c)) // It's not an id if the value is not alpha, numeric or _ and ignore whitespace
            isIdentifier = 0;
    }

    buffer[i] = '\0'; // Null-terminate the string

    // Print the token buffer
    strlcpy(t->value, buffer, MAX_INPUT_SIZE - 1);

    // Check if the buffer matches any keyword
    isKeyword = match_keyword(t, buffer);

    if (!isKeyword && isIdentifier)
    {
        set_token(t, T_IDENTIFIER, buffer);
        putback(c);
        return t->token;
    }

    if (!isDelimiter && !isKeyword && !isIdentifier && c != EOF)
        return reportLexerError(buffer, cs.line, cs.column);

    return t->token;
}

int scan(Token* t)
{
    if (!csIsFileOpen())
        printf("Lexer could not find input");

    int c = skip();

    return match_identifier(t, c);
}

CLoc findErrorLoc(int pos){
    int column = 1;
    int line = 1;
    rewind(cs.infile);
    for(int i = 0; i < pos; i++){
        int c = fgetc(cs.infile);
        if(c == '\n'){
            ++line;
            column = 1;
        }
        else
            ++column;
    }
    return (CLoc){.line = line, .column = column};
}
