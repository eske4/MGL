#include "config.c"
#include "file_config.c"
#include <stdio.h>

extern "C" {

#include "compiler.h"
}

int main() {
  printf("Project Name: %s\n", project_name);
  printf("Project Version: %s\n", project_version);

  compile(SOURCE_CODE_DIR);
  return 0;
}
