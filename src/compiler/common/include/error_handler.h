#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

extern bool isTestMode;
void setTestMode(bool mode);
int reportLexerError(const char *m, const char *input, size_t l, size_t c);

#ifdef __cplusplus
}
#endif
