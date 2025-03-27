#include "lexer.h"
#include "parser.h"
#include "config.c"
#include <iostream>

int main() {
    printf("Project Name: %s\n", project_name);
    printf("Project Version: %s\n", project_version);


    const char *input = "Map TutorialDungeon \n { Room Entrance; Room Hallway; Connect (Entrance -> Hallway); }";

    // Testing parser here.
    int result = parse(input);

    std::cout << "Parsing result: " << result << std::endl;

    // Testing the lexer directly:
    // int token = tokenize(input);

    // std::cout << "Token is " << token << std::endl;  // Print the token
    return 0;
}

