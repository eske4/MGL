#include "config.c"
#include "file_config.c"
#include <stdio.h>

extern "C" {
#include "compiler.h"
}

int main() {
  compile(SOURCE_CODE_FILE);
  return 0;
}
