#pragma once

#include "definitions.h"

typedef struct TypeTableEntry {
    const char* id;
    AbstractTokenDef token;
} TypeTableEntry;

typedef struct TypeTableStruct {
    TypeTableEntry* entries;       // First entry in table
    size_t count;               // Number of entries
    size_t capacity; 
} *TypeTable;
