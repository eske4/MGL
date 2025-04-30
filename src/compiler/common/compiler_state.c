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

int addStringToFile(const char* str)
{
    FILE* temp = tmpfile();

    if (!temp)
    {
        perror("Failed to create temporary file");
        return 0;
    }

    fprintf(temp, "%s", str);

    rewind(temp);
    cs.infile = temp;
    return 1;
}

CLoc findLoc(int pos)
{
    CLoc loc = {1, 1};
    if (!cs.infile || pos < 0)
        return loc;

    rewind(cs.infile);
    for (int i = 0; i <= pos; i++)
    {
        int c = fgetc(cs.infile);
        switch (c)
        {
            case EOF: break;

            case '\n':
                ++loc.line;
                loc.column = 1;
                break;

            case '\t': loc.column += 4 - (loc.column - 1) % 4; break;
            default: ++loc.column;
        }
    }
    return loc;
}
