#include "config.c"
extern "C" {
#include "astree.h"
#include "compiler_state.h"
#include "lexer.h"
#include "parser.h"
}

int main() {
  printf("Project Name: %s\n", project_name);
  printf("Project Version: %s\n", project_version);

  // Token structure to hold the current token
  csInit();
  csOpenFile("/home/eske/Documents/git/MGL/app/test_input.txt");
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

  // Free the AST
  ASTFree(tree);

  // std::cout << "Token is " << token << std::endl;  // Print the token
  return 0;
}
