#pragma once

#include "instr_table.h"

#define MAX_PATH_SIZE 10000

typedef struct Room
{
    char* id;
    char** connections;
    int connCount;
} Room;

const static int MAX_CONNECTIONS = 8;
const static int ID_LEN       = 32;




void generate_assembly(InstructionTable table);
