#include "lexer.h"
#include "config.c"
#include <iostream>

int main() {
    printf("Project Name: %s\n", project_name);
    printf("Project Version: %s\n", project_version);


    const char *input = "yeet";
    int token = tokenize(input);  // Call the tokenize function

    std::cout << "Token is " << token << std::endl;  // Print the token
    return 0;
}

