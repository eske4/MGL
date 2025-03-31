#pragma once
#include <stdio.h>

typedef struct {
  FILE *infile; // The input file
  int line;     // Current line number
  int column;   // Current column number
  int pos;      // Current position in the file
  int putback;  // Position for 'putback' functionality (optional)
} CompilerState;

extern CompilerState cs;

// Function declarations
void csInit();                        // Initialize compiler state
int csOpenFile(const char *filename); // Open file for processing
void csFree();                        // Free resources
int csIsFileOpen();
