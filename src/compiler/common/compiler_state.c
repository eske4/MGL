#include "compiler_state.h"
#include <stdio.h>
#include <stdlib.h>

CompilerState cs;

void csInit()
{
    cs.pos     = 1;
    cs.putback = 0;
    cs.infile  = NULL;
}

int csOpenFile(const char* filename)
{
    cs.infile = fopen(filename, "r");
    if (cs.infile == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

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

CLoc findLoc(int pos) {
    CLoc loc = { .line = 1, .column = 1 };

    if (!cs.infile || pos < 0) {
        return loc; // fallback: start of file
    }

    rewind(cs.infile);

    for (int i = 0; i < pos; i++) {
        int c = fgetc(cs.infile);
        if (c == EOF) {
            break; // stop if end of file reached early
        }
        if (c == '\n') {
            loc.line++;
            loc.column = 1;
        } else {
            loc.column++;
        }
    }

    return loc;
}

