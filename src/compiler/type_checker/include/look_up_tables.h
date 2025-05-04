#pragma once  // ensure header file is only included once in compilation 

#include "definitions.h" 
#include "astree.h"

//=====================================symboltable====================================
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

//=====================================================================================

//=================================Constraint table====================================

//structur to represent a room 
typedef struct {
    const char* room_id;
    int connect_count; 
} ConstrTableRoom; 

//structure to hold information about each map function 
typedef struct {
    const char* map_id;
    int room_constr;
    int connect_constr;
    int room_constr_value;
    int connect_constr_value;
    size_t room_count;
    size_t capacity;
    ConstrTableRoom* rooms; 
} ConstrTableMap;

//structore for constraint table
typedef struct { 
    size_t map_count;
    size_t capacity;
    ConstrTableMap* maps;
} ConstrTable; 

//constraint table helper functions: 
ConstrTable InitConstrTable(size_t initial_capacity); 
void AddMap(ConstrTable* table, const char* map_id);
void AddRoomToMap(ConstrTable* table, const char* map_id, const char* room_id);
void IncrementConnectCount(ConstrTable* table, const char* map_id, const char*  room_id);
void SetRoomConstr(ConstrTable* table, const char*  map_id, int value);
void SetConnectConstr(ConstrTable* table, const char*  map_id, int value);
int CheckRoomConstr (ConstrTable* table);
int CheckConnectConstr(ConstrTable* table);
int FindMaxConnectionCount(const ConstrTable* table);
void FreeConstrTable(ConstrTable* table);
