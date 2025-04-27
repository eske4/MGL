#pragma once

#include "instr_table.h"
#include "definitions.h"

#define MAX_CONNECTIONS 64

#define GENERATED_ASM_DIR ""

typedef struct Room
{
    char* id;
    char** connections;
    int connCount;
} Room;



void generate_assembly(InstructionTable table);
