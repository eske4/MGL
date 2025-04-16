#include "lexer.h"
#include "compiler_state.h"
#include "definitions.h"
#include "error_handler.h"
#include "string_util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set_token(Token* t, TokenDef type, const char* value, int pos)
{
    if (!t || !value || *value == '\0')
        return 0;

    t->token = type;
    t->pos = pos;
    int status = safe_strcpy(t->value, value, MAX_INPUT_SIZE);

    return status;
}

static void putback(int c)
{
    cs.putback = c;
    cs.pos--;
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

    cs.pos++;

    return c;
}

static int skip(void)
{
    int c;
    while ((c = next()) && isspace(c));
    return c;
}

int match_keyword(Token* currentToken, const char* in, int pos)
{

    if(in == NULL || currentToken == NULL) return 0;
    size_t keyword_count = sizeof(keyword_map) / sizeof(keyword_map[0]);
    for (int i = 0; i < keyword_count; i++)
    {
        if (strcmp(in, keyword_map[i].name) == 0)
        {
            set_token(currentToken, keyword_map[i].token, keyword_map[i].name, pos);

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

int match_identifier(Token* t, int c, int startPos)
{

    int i                       = 0;
    char buffer[MAX_INPUT_SIZE] = {0};
    int isIdentifier            = 1;
    int isKeyword               = 0;

    while (!strchr(" \t\n\r\f", c) && i < MAX_INPUT_SIZE - 1)
    {
        buffer[i++] = c;


        if (!(isalnum(c) || c == '_') && !isspace(c)) // It's not an id if the value is not alpha, numeric or _ and ignore whitespace
            isIdentifier = 0;

        if (is_delimiter(c))
        {
            break;
        }

        c = next();

        if (is_delimiter(c))
        {
            putback(c);
            break;
        }

    }

    buffer[i] = '\0'; // Null-terminate the string

    // Check if the buffer matches any keyword
    isKeyword = match_keyword(t, buffer, startPos);


    if(c == EOF){
        set_token(t, T_EOF, "EOF", startPos);
        return t->token;
    }

    if(isKeyword){
        return t->token;
    }

    if (isIdentifier)
    {
        set_token(t, T_IDENTIFIER, buffer, startPos);
        putback(c);
        return t->token;
    }

    if (!isKeyword && !isIdentifier)
    {
        const char *msg[] = {"Invalid token: ", buffer};
        return reportError(ERR_LEXER, startPos, msg, 2); 
    }

    return t->token;
}

int scan(Token* t)
{
    if (!csIsFileOpen())
        printf("Lexer could not find input");

    int c = skip();
    int startPos = cs.pos;
    return match_identifier(t, c, startPos);
}


