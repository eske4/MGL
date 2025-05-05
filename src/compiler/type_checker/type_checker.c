// Included header files:
#include "type_checker.h"
#include "error_handler.h"
#include "look_up_tables.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration to let program know these function/declaration exist (top-down compiled )
void checkMap(SymbolTable table, const char *id, const ASTNode *node);
void checkRoom(SymbolTable table, const char *id, const ASTNode *node);
void checkRoomsInConnection(char *id, char *id2, SymbolTable table, const ASTNode *node);
void checkConnection(SymbolTable table, const char *id, const ASTNode *node);
void checkConnectConstr(SymbolTable table, int value);
void reportSemanticError(ErrorCode err, int pos, const char* msg);
void PrintSymbolTable(const SymbolTable table);

static char current_map_id[MAX_INPUT_SIZE];             // Varibale to keep track of what Map is being treversed 
int  max_connections_global = 2;                        // Initilize global variable for max connection ("definitions.h")

// Function to treverse AST and add to symboltable
void TraverseAST(const ASTNode* node, const SymbolTable table, ConstrTable* constr_table){
    if(!node) return; //safty to check if node exist in AST

    switch(node->type){  
        case AT_MAP:
            // Check if Map exist in symbol table
            checkMap(table, node->children[0]->data, node);  
            
            // Let the file know what Map it handels (update "current_map_id" variable)
            strncpy(current_map_id, node->children[0]->data, sizeof(current_map_id) - 1);
            current_map_id[sizeof(current_map_id) - 1] = '\0'; 

            // Add to symbol and constraint table
            AddSymbolTable(table, node->children[0]->data, node); 
            AddMap(constr_table, current_map_id);
            break;

        case AT_MAP_CONSTR_ROOMS:
            SetRoomConstr(constr_table, current_map_id, atoi(node->children[0]->data));
            break; 
        
        case AT_MAP_CONSTR_CONNECT: 
            SetConnectConstr(constr_table, current_map_id, atoi(node->children[0]->data));   
            break;

        case AT_ROOM:
            checkRoom(table, node->children[0]->data, node);
            AddSymbolTable(table, node->children[0]->data, node); 
            AddRoomConstrTable(constr_table, current_map_id, node->children[0]->data);  
            break;
            
        case AT_CONNECT:
            // Define unique connection name for connection 
            char* connection_id = calloc(1, strlen(node->children[0]->data) + strlen(node->children[2]->data) + 2);
            sprintf(connection_id, "#%s%s", node->children[0]->data, node->children[2]->data);

            checkConnection(table, connection_id, node);
            checkRoomsInConnection(node->children[0]->data, node->children[2]->data, table, node);
            AddSymbolTable(table, connection_id, node);

            IncrementConnectCount(constr_table, current_map_id, node->children[0]->data);
            if (node->children[1]->type == T_BIDIRECTIONAL_EDGE) IncrementConnectCount(constr_table, current_map_id, node->children[2]->data);
            break;

        default: break;
        
    }

    // Recursively go throug each node in AST
    for (int i = 0; i < node->child_count; i++)
    {
        TraverseAST(node->children[i], table, constr_table);
    }
}

// Main function for semantic check 
int TypeCheck(const ASTree tree)
{
    // Initiate the symbol and constraint tables
    SymbolTable table = InitSymbolTable(20);            
    ConstrTable constr_table = InitConstrTable(2);      

    // Treverse Abstract syntax tree from top node
    ASTNode* root = tree->head;                         
    TraverseAST(root, table, &constr_table); 

    // Functions to do constraint checks
    if (CheckRoomConstr(&constr_table)==0) reportSemanticError(ERR_SEMANTIC, 0, "Rooms exceed constraint limit");
    if (CheckConnectConstr(&constr_table)==0) reportSemanticError(ERR_SEMANTIC, 0, "Room connections exceeds constraint limit");
    // Define the max connection for a room (for code gen)
    max_connections_global = FindMaxConnectCount(&constr_table, 2);

    // Print Symbol table
    PrintSymbolTable(table); 

    // Free both tables from memory
    FreeSymbolTable(table);
    FreeConstrTable(&constr_table); 
    return 1;
}

// Function to check if Map is part of symboltable
void checkMap(SymbolTable table, const char *id, const ASTNode* node){
    const SymbolTableEntry *val = LookUpSymbolTable(table, id);
    if(val != NULL) //if already part of symbol table - report semantic error 
        if(val->ast_location->type == AT_MAP)
            reportSemanticError(ERR_SEMANTIC, node->pos, "Map is already declared");
        else {
            char msg[128];
            snprintf(msg, sizeof(msg), "map name is already declared for type: %d", val->ast_location->type);
            reportSemanticError(ERR_SEMANTIC, node->pos, msg);
        }
}

// Function to check if Room is part of symboltalbe
void checkRoom(SymbolTable table, const char* id, const ASTNode* node)
{
    const SymbolTableEntry* val = LookUpSymbolTable(table, id);
    if (val != NULL) //if already part of symbol table - report semantic error
        if (val->ast_location->type == AT_ROOM)
            reportSemanticError(ERR_SEMANTIC, node->pos, "Room is already declared");
        else
        {
            char msg[128];
            snprintf(msg, sizeof(msg), "Room name is already declared for type: %d", val->ast_location->type);
            reportSemanticError(ERR_SEMANTIC, node->pos, msg);
        }
}

// Function to check if Connection is part of symboltalbe
void checkConnection(SymbolTable table, const char* id, const ASTNode* node)
{
    const SymbolTableEntry* val = LookUpSymbolTable(table, id);
    if (val != NULL) //if already part of symbol table - report semantic error
        reportSemanticError(ERR_SEMANTIC, node->pos, "Connection is already declared");
}

// Function to check for if Rooms in connection is part of symboltable
void checkRoomsInConnection(char* id, char* id2, SymbolTable table, const ASTNode* node)
{
    //lookup rooms in symboltable
    const SymbolTableEntry* idRef  = LookUpSymbolTable(table, id);
    const SymbolTableEntry* id2Ref = LookUpSymbolTable(table, id2);

    //check for undeclared rooms in connection
    if (!idRef || !id2Ref)
        reportSemanticError(ERR_SEMANTIC, node->pos, "Cant connect undeclared Room reference");

    //check if Rooms are of correct type
    if (idRef->ast_location->type != AT_ROOM || id2Ref->ast_location->type != AT_ROOM)
        reportSemanticError(ERR_TYPE, node->pos, "Expected Room types");

    //check if rooms are the same -- (maybe not an error in future since it could be a feature in game)
    if (strcmp(idRef->id, id2Ref->id) == 0)
        reportSemanticError(ERR_SEMANTIC, node->pos, "Can't connect a Room to itself");
}

// Error message function 
void reportSemanticError(ErrorCode err, int pos, const char* msg){
    const char *message[] = {msg};
    reportError(err, pos, message, 1);
}

// Function to print the symboltable
void PrintSymbolTable(const SymbolTable table) {
    if (!table) return;

    printf("\n Symbol Table (%zu entries):\n", table->count);
    for (size_t i = 0; i < table->count; i++) {
        SymbolTableEntry entry = table->entries[i];
        printf("  ID: %s, AST location: %p \n",
               entry.id,
               (void*)entry.ast_location 
            );  
    }
}
