#pragma once

#ifdef __cplusplus // Extern C fix linking problem from c code with c++ compiler
                   // leaving the option for c++
extern "C" {
#endif

void lexer(const char *input);

#ifdef __cplusplus
}
#endif
