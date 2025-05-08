#pragma once

//–– Our operation codes, add more as our DSL grows ––
typedef enum {
    IR_DECL_MAP,
    IR_DECL_ROOM,
    IR_DECL_CONNECT,
} InstructionCode;

//–– A single IR instruction ––  
typedef struct {
    InstructionCode InstrCode;    // which instruction it is
    char **args;    // dynamic array of C‑strings["x", "y", "z"]
    int argc;       // number of args
} Instruction;

//–– The entire IR program –– 
typedef struct {
    Instruction *entries; // dynamic array of Instructions
    int count;
    int capacity;
} *InstructionTable;

InstructionTable instr_table_init(); // Initialize and return a new, empty instruction table.
Instruction instr_create(InstructionCode IRCode, char **args, int argc); // Create an instruction with the given code and argument list.
void instr_add(InstructionTable table, Instruction instr); // Append the specified instruction to the instruction table.
void instr_free(InstructionTable table); // Free all memory associated with the instruction table and its entries.
void instr_print(InstructionTable table); // Print out the IL instructions in a human-readable format.