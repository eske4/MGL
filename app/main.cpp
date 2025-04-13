#include "config.c"
extern "C" {
#include "astree.h"
#include "compiler_state.h"
#include "lexer.h"
#include "parser.h"
#include "ir_generator.h"
}

int main() {
  printf("Project Name: %s\n", project_name);
  printf("Project Version: %s\n", project_version);

  // Token structure to hold the current token
  csInit();
  csOpenFile("test_input.txt");
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

  // Generate IR code from the AST.
  IRArray ir;
  compileToIR(tree, &ir);
  printIR(&ir);

  // Free the AST
  ASTFree(tree);

  // Free IR
  freeIRArray(&ir);

  // std::cout << "Token is " << token << std::endl;  // Print the token
  return 0;
}
