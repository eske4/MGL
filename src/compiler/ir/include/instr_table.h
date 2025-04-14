#pragma once

typedef enum {
    IR_DECL_MAP,
    IR_DECL_ROOM,
    IR_DECL_CONNECT,
} InstructionCode;

typedef struct {
    InstructionCode IRCode;    // instruction opcode as a string, e.g. "add", "mov", "jmp"
    char **args;    // array of strings like ["x", "y", "z"]
    int argc;       // number of args
} Instruction;

typedef struct {
    Instruction *entries;
    int count;
    int capacity;

} *InstructionTable;

InstructionTable IRInit();
Instruction IRMakeInstr(InstructionCode IRCode, char **args, int argc);
void IRAddInstr(InstructionTable table, Instruction instr);
void IRFree(InstructionTable table);
void IRPrint(InstructionTable table);


