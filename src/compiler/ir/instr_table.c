#include "instr_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
InstructionTable IRInit(){
    InstructionTable table = calloc(1, sizeof(*table));
    table->entries = calloc(1, sizeof(Instruction));
    table->count = 0;
    table->capacity = 0;
    return table;
}
Instruction IRMakeInstr(InstructionCode IRCode, char **args, int argc) {
    Instruction instr;
    instr.IRCode = IRCode;
    instr.argc = argc;
    instr.args = calloc(argc, sizeof(char *));
    for (int i = 0; i < argc; i++) {
        instr.args[i] = strdup(args[i]);  // deep copy to avoid shared pointers
    }
    return instr;
}
void IRAddInstr(InstructionTable table, Instruction instr) {
    if (table->count >= table->capacity) {
    table->capacity = table->capacity == 0 ? 2 : table->capacity * 2;
    table->entries = realloc(table->entries, table->capacity * sizeof(Instruction));
    if (!table->entries) {
        fprintf(stderr, "Failed to realloc entries\n");
        exit(1);
    }
}

    table->entries[table->count++] = instr;
}

void IRFree(InstructionTable table) {
    for (int i = 0; i < table->count; i++) {
        Instruction *instr = &table->entries[i];
        for (int j = 0; j < instr->argc; j++) {
            free(instr->args[j]);  // Free each argument string
        }
        free(instr->args);  // Free the array of arguments
    }
    free(table->entries);  // Free the instruction entries array
    free(table);  // Free the table itself
}

void IRPrint(InstructionTable table) {
    for (int i = 0; i < table->count; i++) {
        Instruction *instr = &table->entries[i];
        switch(instr->IRCode){
            case IR_DECL_MAP:
                printf("Map instr %s\n", instr->args[0]); break;
                break;
            case IR_DECL_ROOM:
                printf("Room instr %s\n", instr->args[0]);
                break;
            case IR_DECL_CONNECT:
                printf("Connect instr ");
                for (int i = 0; i < instr->argc; i++) {
                    printf("%s ", instr->args[i]);
                
                }
                printf("\n");
                break;

        }
    }
}


