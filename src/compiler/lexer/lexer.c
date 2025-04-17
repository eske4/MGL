#include "lexer.h"
#include "compiler_state.h"
#include "definitions.h"
#include "error_handler.h"
#include "string_util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void putback(int c);
static int next(void);
static int skip(void);

int set_token(Token* t, TokenDef type, const char* value, int pos);
int match_map(const Map map[], const char* key, const int pos, Token* token, const size_t size);
int tokenize(Token* t, int c, int startPos);
int is_delimiter(int c);

////////////////////////////////////////
///           Main function          ///
////////////////////////////////////////


int scan(Token* t){
    if (!csIsFileOpen())
    {
        perror("Lexer could not find input");
        return -1;
    }

    int c = skip();
    int startPos = cs.pos;
    return tokenize(t, c, startPos);
}

////////////////////////////////////////
///           Move functions         ///
////////////////////////////////////////

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

////////////////////////////////////////
///           Main logic             ///
////////////////////////////////////////

int tokenize(Token* t, int c, int startPos)
{
    int i                       = 0;
    char buffer[MAX_INPUT_SIZE] = {0};
    int isIdentifier            = 1;

    while (!strchr(" \t\n\r\f", c) && i < MAX_INPUT_SIZE - 1)
    {
        buffer[i++] = c;


        if (!(isalnum(c) || c == '_') && !isspace(c)) // It's not an id if the value is not alpha, numeric or _ and ignore whitespace
            isIdentifier = 0;

        if (is_delimiter(c))
            break;

        c = next();

        if (is_delimiter(c))
        {
            putback(c);
            break;
        }

    }

    buffer[i] = '\0'; // Null-terminate the string

    // Check if the buffer matches any keyword


    if(c == EOF){
        set_token(t, T_EOF, "EOF", startPos);
        return t->token;
    }

    if(match_map(delimiter_map, buffer, startPos, t, delimiter_size))
        return t->token;

    if(match_map(keyword_map, buffer, startPos, t, keyword_size))
        return t->token;

    if (isIdentifier)
    {
        set_token(t, T_IDENTIFIER, buffer, startPos);
        putback(c);
        return t->token;
    }

    const char *msg[] = {"Invalid token: ", buffer};
    return reportError(ERR_LEXER, startPos, msg, 2); 
}

////////////////////////////////////////
///           Helper functions       ///
////////////////////////////////////////

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


int match_map(const Map map[], const char* key, const int pos, Token* token, const size_t size){
    if(key == NULL || token == NULL) return 0;
    for(int i = 0; i < size; i++){
        if(strcmp(key, map[i].name) == 0){
            set_token(token, map[i].token, map[i].name, pos);
            return 1;
        }
    }
    return 0;
}

int set_token(Token* t, TokenDef type, const char* value, int pos)
{
    if (!t || !value || *value == '\0')
        return 0;

    t->token = type;
    t->pos = pos;
    int status = safe_strcpy(t->value, value, MAX_INPUT_SIZE);

    return status;
}
