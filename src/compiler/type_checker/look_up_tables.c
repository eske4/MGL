#include "look_up_tables.h"
#include <string.h>
#include <stdlib.h>


//linear search for entries with matching ids 
const SymbolTableEntry* LookUpSymbolTable(const SymbolTable table, const char* id) {
    if (!table || !id) return NULL;  // Safety check (to see if there exist an table or id)

    for (size_t i = 0; i < table->count; i++) { //use for loop to go through each element in the SymbolTable table
        if (strcmp(table->entries[i].id, id) == 0) {  //use strcmp (string compare) to see if the id match (returns 0 if equal)
            return &table->entries[i];  //Return address of entry if match
        }
    }
    return NULL; // if not found function return null
}

//add entry to symbol table (identifier + location)
void AddSymbolTable(SymbolTable table, const char* id, const ASTNode* node) {
    // if statement to grow capasity of symboltable if needed
    if (table->count >= table->capacity) { //check if the number entries in symbol table have reached the capacity
        size_t new_cap = table->capacity * 2;   //if the count is more/equal to capacity, define new capsity double the table capacity
        SymbolTableEntry* new_entries = realloc(table->entries, new_cap * sizeof(SymbolTableEntry)); //uses the realloc (re allocate) function to reserve memory for the new cap (new_cap times the memory of one symboltable entry)
        table->entries = new_entries; //update the entris in symboltable to new momory block returned by the realloc function
        table->capacity = new_cap; //update the capacity of the symboltable to the new_cap
    }

    // add 1 more more entry to symboltable with the id and location
    table->entries[table->count++] = (SymbolTableEntry){
        .id = id,
        .ast_location = node,
    };
}

//allocates and initilizes a new symbol table
SymbolTable InitSymbolTable(size_t initial_capacity) { //function takes argument initial_capacity (how many entries the table should be able to hold)
    SymbolTable table = calloc(1, sizeof(*table)); //use calloc function (contiguous allocation) to create space for 1 entry with the size of the table
    table->entries = calloc(1, initial_capacity * sizeof(SymbolTableEntry)); 
    table->count = 0;
    table->capacity = initial_capacity;
    return table;
}

//clean up memory
void FreeSymbolTable(SymbolTable table) {
    free(table->entries);
    free(table);
}


