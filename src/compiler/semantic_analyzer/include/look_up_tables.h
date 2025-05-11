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

//=================================Constraint table====================================

// Structur to represent a room 
typedef struct {
    const char* room_id;                // Room id 
    int connect_count;                  // Number of connection to other rooms
} ConstrTableRoom; 

// Structure to hold information about each map function 
typedef struct {
    const char* map_id;                 // Map id
    int room_constr;                    // Bool for if max room constraint is activated 
    int connect_constr;                 // Bool for if max connection constraint is activated 
    int room_constr_value;              // Number of rooms allowed by max room constraint
    int connect_constr_value;           // Number of connections allowed by max connection constraint
    size_t room_count;                  // Number of rooms in map
    size_t capacity;                    // Number of rooms the struct can hold 
    ConstrTableRoom* rooms;             // Rooms in map
} ConstrTableMap;

// Structore for constraint table
typedef struct {                        
    size_t map_count;                   // Number of maps 
    size_t capacity;                    // Number of maps the struct can hold
    ConstrTableMap* maps;               // Maps in the struct
} ConstrTable; 

// Constraint table helper functions (see logic in look_up_tables.c): 
ConstrTable InitConstrTable(size_t initial_capacity); 
void AddMap(ConstrTable* table, const char* map_id);
void AddRoomConstrTable(ConstrTable* table, const char* map_id, const char* room_id);
void IncrementConnectCount(ConstrTable* table, const char* map_id, const char*  room_id);
void SetRoomConstr(ConstrTable* table, const char*  map_id, int value);
void SetConnectConstr(ConstrTable* table, const char*  map_id, int value);
int CheckRoomConstr (ConstrTable* table);
int CheckConnectConstr(ConstrTable* table);
int FindMaxConnectCount(const ConstrTable* table, int max_connect);
void FreeConstrTable(ConstrTable* table);
