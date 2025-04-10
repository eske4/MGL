#include "declaration_tracker.h"
#include "definitions.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

TypeTable TTInit(size_t initial_capacity);
void TTFree(TypeTable table);
void TTAdd(TypeTable table, const char* id, AbstractTokenDef token);
const TypeTableEntry* TTFind(const TypeTable table, const char* id);

const TypeTableEntry* TTFind(const TypeTable table, const char* id) {
    if (!table || !id) return NULL;  // Safety check
    
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].id, id) == 0) {  // FIX 1: String comparison
            return &table->entries[i];  // FIX 2: Return address of entry
        }
    }
    return NULL;
}

void TTAdd(TypeTable table, const char* id, AbstractTokenDef token) {
    // Grow if needed
    if (table->count >= table->capacity) {
        size_t new_cap = table->capacity * 2;
        TypeTableEntry* new_entries = realloc(table->entries, new_cap * sizeof(TypeTableEntry));
        table->entries = new_entries;
        table->capacity = new_cap;
    }
    table->entries[table->count++] = (TypeTableEntry){ .id = id, .token = token };
}

TypeTable TTInit(size_t initial_capacity) {
    TypeTable table = calloc(1, sizeof(*table));
    table->entries = calloc(1, initial_capacity * sizeof(TypeTableEntry));
    table->count = 0;
    table->capacity = initial_capacity;
    return table;
}

void TTFree(TypeTable table) {
    free(table->entries);
    free(table);
}

void TraverseAST(const ASTNode* node, const TypeTable table){
    if(!node) return;

    switch(node->type){
        case AT_MAP:
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;

        case AT_ROOM:
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;
        case AT_CONNECT:
            const TypeTableEntry *id = TTFind(table, node->children[0]->data.stringVal);
            const TypeTableEntry *id2 = TTFind(table, node->children[2]->data.stringVal);
            if(!id || !id2) fprintf(stderr, "Connect in wrong order\n"); exit(1);
        default: break;
    }

    for(int i = 0; i < node->child_count; i++){
        TraverseAST(node->children[i], table);
    }
}

void AnalyseAST(const ASTree tree){
    TypeTable table = TTInit(20);
    ASTNode* root = tree->head;
    TraverseAST(root, table);
    TTFree(table);
}

