#pragma once

#include "instr_table.h"
#include "definitions.h"

#define MAX_CONNECTIONS 64

const char assembly_file[MAX_INPUT_SIZE] = "map_gen";

void generate_assembly(InstructionTable table);
