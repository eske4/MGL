#pragma once

//–– Our operation codes, add more as our DSL grows ––
typedef enum {
    IR_DECL_MAP,
    IR_DECL_ROOM,
    IR_DECL_CONNECT,
} InstructionCode;

typedef struct {
    InstructionCode InstrCode;    // instruction opcode as a string, e.g. "add", "mov", "jmp"
    char **args;    // array of strings like ["x", "y", "z"]
    int argc;       // number of args
} Instruction;

typedef struct {
    Instruction *instructions;
    int count;
    int capacity;

} *InstructionTable;

InstructionTable InstrInit();
Instruction InstrMake(InstructionCode IRCode, char **args, int argc);
void InstrAdd(InstructionTable table, Instruction instr);
void InstrFree(InstructionTable table);
void InstrPrint(InstructionTable table);


