#pragma once

#include <stddef.h>
#define MAX_ERROR_LENGTH 256 

typedef enum {
    ERR_UNKNOWN = 0,
    ERR_LEXER,
    ERR_PARSER,
    ERR_SEMANTIC,
    ERR_TYPE,
    ERR_COUNT // Always has to be at the bottom of this enum
} ErrorCode;

typedef struct CLoc {
    int line;
    int column;
} CLoc;

extern const char *const ERROR_LABELS[ERR_COUNT];
extern int isTestMode;

void setTestMode(int mode);
int reportError(ErrorCode code, int pos, const char *strings[], size_t msgc);
