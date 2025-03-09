#include "lexer.h" // Include the iostream library for input and outpu
#include "config.c"
#include <stdio.h>
int main() { // Entry point of the program
    printf("Project Name: %s\n", project_name);
    printf("Project Version: %s\n", project_version);

    lexer("Hello, Lexer!");
    return 0;
}
