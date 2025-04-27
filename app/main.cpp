#include "config.c"
#include "file_config.c"

extern "C" {
#include "astree.h"
#include "compiler_state.h"
#include "lexer.h"
#include "parser.h"
#include "type_checker.h"
#include "il.h"
#include "code_gen.h"
}

int main() {
  printf("Project Name: %s\n", project_name);
  printf("Project Version: %s\n", project_version);

  // Token structure to hold the current token
  csInit();
  csOpenFile(SOURCE_CODE_DIR);
  Token currentToken;

  // Fetch the first token

  scan(&currentToken);

  // Parse the input and construct the AST
  ASTree tree = parse(&currentToken);

  if (tree && tree->head) {
    printf("Parsing successful!\n");
    ASTreePrint(tree);
  } else {
    printf("Parsing failed!\n");
  }

  TypeCheck(tree);
  InstructionTable ilTable = compile_to_il(tree);
  instr_print(ilTable);

  generate_assembly(ilTable);


  // Free the AST
  ASTFree(tree);
  // Free IL
  instr_free(ilTable);


  // std::cout << "Token is " << token << std::endl;  // Print the token
  return 0;
}
