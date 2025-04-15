#pragma once
#include <stdio.h>
#include "error_handler.h"

typedef struct {
  FILE *infile; // The input file
  int pos;      // Current position in the file
  int putback;  // Position for 'putback' functionality (optional)
} CompilerState;

extern CompilerState cs;

// Function declarations
void csInit();                        // Initialize compiler state
int csOpenFile(const char *filename); // Open file for processing
void csFree();                        // Free resources
int csIsFileOpen();
CLoc findLoc(int pos);

