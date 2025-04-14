#pragma once

#include "definitions.h"

typedef struct SymbolTableEntry {
    const char* id;
    AbstractTokenDef token;
    char value[MAX_INPUT_SIZE];
} SymbolTableEntry;

typedef struct SymbolTableStruct {
    SymbolTableEntry* entries;       // First entry in table
    size_t count;               // Number of entries
    size_t capacity; 
} *SymbolTable;

SymbolTable TTInit(size_t initial_capacity);
void TTFree(SymbolTable table);
void TTAdd(SymbolTable table, const char* id, AbstractTokenDef token);
const SymbolTableEntry* TTLookUp(const SymbolTable table, const char* id);


