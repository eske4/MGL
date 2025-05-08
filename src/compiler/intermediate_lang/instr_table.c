#include "instr_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InstructionTable instr_table_init(){
    InstructionTable table = calloc(1, sizeof(*table));
    table->entries = calloc(1, sizeof(Instruction));
    table->count = 0;
    table->capacity = 0;
    return table;
}

Instruction instr_create(InstructionCode IRCode, char **args, int argc) {
    Instruction instr;
    instr.InstrCode = IRCode;
    instr.argc = argc;
    instr.args = calloc(argc, sizeof(char *));
    for (int i = 0; i < argc; i++) {
        instr.args[i] = strdup(args[i]);  // deep copy to avoid shared pointers
    }
    return instr;
}

void instr_add(InstructionTable table, Instruction instr) {
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

void instr_free(InstructionTable table) {
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

void instr_print(InstructionTable table) {
    printf("\nIntermediate Language (IL):\n");
    for (int i = 0; i < table->count; i++) {
        Instruction *instr = &table->entries[i];
        switch(instr->InstrCode){
            case IR_DECL_MAP:
                printf("IR_DECL_MAP: %s\n", instr->args[0]); break;
                break;
            case IR_DECL_ROOM:
                printf("IR_DECL_ROOM: %s\n", instr->args[0]);
                break;
            case IR_DECL_CONNECT:
                printf("IR_DECL_CONNECT: ");
                for (int i = 0; i < instr->argc; i++) {
                    printf("%s ", instr->args[i]);
                }
                printf("\n");
                break;
        }
    }
}
