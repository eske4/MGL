#include "file_config.c"

extern "C" {
#include "compiler.h"
}

int main() {
  compile(SOURCE_CODE_FILE);
  return 0;
}
