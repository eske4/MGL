#pragma once  // ensure header file is only included once in compilation 

#include "astree.h"

//declare which structs and function are used for the look up tables:

//structs:
typedef struct SymbolTableEntry {   //struct for single entry in the symbol table 
    const char* id;                     //entry id        
    const ASTNode* ast_location;        //pointer to AST node
} SymbolTableEntry;

typedef struct SymbolTableStruct {  //struct for entire symboltable
    SymbolTableEntry* entries;          // First entry in table
    size_t count;                       // Number of entries
    size_t capacity;                    // maximum number of entries to be hold without reaallocating
} *SymbolTable;

//functions (see logic in look_up_tables.c):
SymbolTable InitSymbolTable(size_t initial_capacity);   
void FreeSymbolTable(SymbolTable table);    
void AddSymbolTable(SymbolTable table, const char* id, const ASTNode* node);
const SymbolTableEntry* LookUpSymbolTable(const SymbolTable table, const char* id);


