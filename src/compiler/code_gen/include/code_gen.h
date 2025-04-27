#pragma once

#include "instr_table.h"

#define MAX_CONNECTIONS 64
#define MAX_PATH_SIZE 10000

typedef struct Room
{
    char* id;
    char** connections;
    int connCount;
} Room;



void generate_assembly(InstructionTable table);
