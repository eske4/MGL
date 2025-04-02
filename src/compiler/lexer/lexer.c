#include "lexer.h"
#include "compiler_state.h"
#include "error_handler.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int set_token(Token* t, TokenDef type, const char* value)
{
    if (!t || !value || *value == '\0')
        return 0;

    t->token = type;

    switch (type)
    {
        case T_INTEGER:
        {
            char* endptr;
            errno       = 0;
            long intVal = strtol(value, &endptr, 10);
            if (errno == ERANGE || *endptr)
                return 0;
            t->value.intValue = intVal;
            break;
        }
        case T_FLOAT:
        {
            char* endptr;
            errno          = 0;
            float floatVal = strtof(value, &endptr);
            if (errno == ERANGE || *endptr)
                return 0;
            t->value.floatValue = floatVal;
            break;
        }
        default: strlcpy(t->value.stringValue, value, sizeof(t->value.stringValue)); break;
    }
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
    while ((c = next()) && strchr(" \t\n\r\f", c))
        ;
    return c;
}

static int match_sequence(int c, const char* seq, TokenDef tokenType, Token* t)
{
    const char* ptr = seq;

    // Go through each character in the sequence
    while (*ptr)
    {
        if (c != *ptr)
        {
            // If there's a mismatch, backtrack and return failure
            putback(c);
            return 0;
        }

        // Move to the next character in the sequence
        ptr++;
        if (*ptr)
        {
            c = next(); // Fetch the next character if we're not at the end
        }
    }

    // If we've gone through the entire sequence successfully
    return set_token(t, tokenType, seq);
}

int match_identifier(Token* t, char c)
{
    int i = 0;
    char buffer[sizeof(t->value.stringValue)];

    if (isalpha(c) || c == '_')
    {
        do
        {
            if (i < sizeof(buffer) - 1)
            {
                buffer[i++] = c;
            }
            else
            {
                break;
            }
            c = next();
        } while (isalnum(c) || c == '_');

        buffer[i] = '\0';
        putback(c);
        return set_token(t, T_IDENTIFIER, buffer);
    }

    reportLexerError("Lexer error: Unrecognised character ", &c, cs.line, cs.column);
    return 0;
}

int scan(Token* t)
{
    if (!csIsFileOpen())
        printf("Lexer could not find input");

    int c = skip();
    switch (c)
    {
        case 'R': return match_sequence(c, "Room", T_ROOM, t); break;
        case 'M': return match_sequence(c, "Map", T_MAP, t); break;
        case 'C': return match_sequence(c, "Connect", T_CONNECT, t); break;
        case '-': return match_sequence(c, "->", T_DIRECTED_EDGE, t); break;
        case '<': return match_sequence(c, "<->", T_BIDIRECTIONAL_EDGE, t); break;

        case ';': return set_token(t, T_SEMICOLON, ";"); break;
        case '{': return set_token(t, T_LBRACE, "{"); break;
        case '}': return set_token(t, T_RBRACE, "}"); break;
        case '(': return set_token(t, T_LPAREN, "("); break;
        case ')': return set_token(t, T_RPAREN, ")"); break;
        case EOF: return 0;
        default: return match_identifier(t, c);
    }
}
