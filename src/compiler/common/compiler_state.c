#include "compiler_state.h"
#include <stdio.h>
#include <stdlib.h>

CompilerState cs;

void csInit()
{
    cs.line    = 1;
    cs.pos     = 0;
    cs.column  = 0;
    cs.line    = 1;
    cs.putback = -1;
    cs.infile  = NULL;
}

int csOpenFile(const char* filename)
{
    cs.infile = fopen(filename, "r");
    if (cs.infile == NULL)
    {
        fprintf(stderr, "Error opening file");
        exit(1);
    }

    cs.line    = 1;
    cs.column  = 0;
    cs.pos     = 0;
    cs.putback = 0;

    printf("File '%s' successfully opened.\n", filename);
    return 1;
}

void csFree()
{
    if (cs.infile != NULL)
    {
        fclose(cs.infile); // Close the file
        cs.infile = NULL;  // Set to NULL to avoid dangling pointer
    }
}

int csIsFileOpen()
{
    return cs.infile != NULL;
}
