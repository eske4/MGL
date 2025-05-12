// Ensure header file is only included once in compilation 
#pragma once

// Included header files:
#include "definitions.h" 
#include "astree.h"

//=====================================Symboltable====================================
// Declare which structs and function are used for the look up tables:

//data structure for each entry in symboltable: 
typedef struct SymbolTableEntry {                 
    const ASTNode* ast_location;        // Pointer to Abstract syntax tree node
} SymbolTableEntry;

//data structure symboltable: 
typedef struct SymbolTableStruct {  
    SymbolTableEntry* entries;          // Entrities in the symboltable
    size_t count;                       // Number of entetries in the symboltable
    size_t capacity;                    // Entetry capacity of the symboltable
    int room_constr;
    int connect_constr; 
} *SymbolTable;

//Helper functions for symboltable (see logic in look_up_tables.c)
SymbolTable InitSymbolTable(size_t initial_capacity);   
void FreeSymbolTable(SymbolTable table);    
void AddSymbolTable(SymbolTable table, const ASTNode* node);
const SymbolTableEntry* LookUpSymbolTable(const SymbolTable table, const char* id);
int LookUpConnectSymbolTable(const SymbolTable table, const char* first_room, const char* second_room);
void ConstrSymbolTable(const SymbolTable table, const ASTNode* node); 
int CheckConstrSymbolTable(SymbolTable table); 

//=====================================================================================
