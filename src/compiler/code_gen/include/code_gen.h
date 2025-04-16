#pragma once

#include "instr_table.h"
#include "definitions.h"

const char assembly_file[MAX_INPUT_SIZE] = "map_gen.asm";

void generate_assembly(InstructionTable table);
