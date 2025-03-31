#include "error_handler.h"
#include <iostream>
#include <string>

bool isTestMode = false;

void setTestMode(bool mode) {
  isTestMode = mode; // Set the global variable
}

int reportError(const char *message, const char *input, size_t line,
                size_t column) {
  // Check if the message is NULL
  if (message == nullptr) {
    std::cerr << "Error: NULL message provided at line " << line << ", column "
              << column << std::endl;
  } else {
    std::string msg(
        message); // Convert c string to c++ to avoid potential memory bugs
    std::cerr << msg << input << " at line " << line << ", column " << column
              << std::endl; // print error message with location
  }

  // Exit the program with an error status
  if (!isTestMode) {
    std::exit(1);
  }
  return 0;
}
