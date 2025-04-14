#include "look_up_tables.h"
#include <string.h>
#include <stdlib.h>

const SymbolTableEntry* TTLookUp(const SymbolTable table, const char* id) {
    if (!table || !id) return NULL;  // Safety check
    
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].id, id) == 0) {  // FIX 1: String comparison
            return &table->entries[i];  // FIX 2: Return address of entry
        }
    }
    return NULL;
}

void TTAdd(SymbolTable table, const char* id, AbstractTokenDef token) {
    // Grow if needed
    if (table->count >= table->capacity) {
        size_t new_cap = table->capacity * 2;
        SymbolTableEntry* new_entries = realloc(table->entries, new_cap * sizeof(SymbolTableEntry));
        table->entries = new_entries;
        table->capacity = new_cap;
    }
    table->entries[table->count++] = (SymbolTableEntry){ .id = id, .token = token };
}

SymbolTable TTInit(size_t initial_capacity) {
    SymbolTable table = calloc(1, sizeof(*table));
    table->entries = calloc(1, initial_capacity * sizeof(SymbolTableEntry));
    table->count = 0;
    table->capacity = initial_capacity;
    return table;
}

void TTFree(SymbolTable table) {
    free(table->entries);
    free(table);
}


