// Included header files:
#include "semantic_analyzer.h"
#include "error_handler.h"
#include "look_up_tables.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration to let program know these function/declaration exist (top-down compiled )
void TraverseAST(const ASTNode* ast_node, const SymbolTable symbol_table);
void checkMap(SymbolTable table, const ASTNode* node);
void checkRoom(SymbolTable table, const ASTNode* node);
void checkConnection(SymbolTable table, const ASTNode* node);
void reportSemanticError(ErrorCode err, int pos, const char* msg);
void PrintSymbolTable(const SymbolTable table);

// Initilize global variable for max connection ("definitions.h")
int max_connections_global = 8;    

// Main function for semantic check
int TypeCheck(const ASTree tree)
{
    // Initiate the symbol table
    SymbolTable table = InitSymbolTable(20);

    // Treverse AST from top node
    ASTNode* root = tree->head;
    TraverseAST(root, table);

    // Define global parameter after number of connections in the map function (ignore constraint value - faster to generate assembly?)
    max_connections_global = CheckConstrSymbolTable(table);

    // Do constraint check of symboltable
    if (max_connections_global == -1) reportSemanticError(ERR_SEMANTIC, 1, "Constraint of Map function violated");

    // Print Symbol table
    PrintSymbolTable(table);

    // Free symbol table from memory
    FreeSymbolTable(table);

    return 1;
}

// Function to treverse AST and add to symboltable
void TraverseAST(const ASTNode* ast_node, const SymbolTable symbol_table)
{
    //safty to check if AST node exist in AST
    if (!ast_node)
        return;

    //Different commands depending on AST node type
    switch (ast_node->type)
    {
        case AT_MAP: 
            checkMap(symbol_table, ast_node);
            AddSymbolTable(symbol_table, ast_node);
            break;

        case AT_MAP_CONSTR_ROOMS: 
            ConstrSymbolTable(symbol_table, ast_node); 
            break;

        case AT_MAP_CONSTR_CONNECT: 
            ConstrSymbolTable(symbol_table, ast_node); 
            break; 

        case AT_ROOM:
            checkRoom(symbol_table, ast_node);
            AddSymbolTable(symbol_table, ast_node);
            break;

        case AT_CONNECT: 
            checkConnection(symbol_table, ast_node); 
            AddSymbolTable(symbol_table, ast_node);
            break;

        default: break;
    }

    // Recursively go throug each node in AST
    for (int i = 0; i < ast_node->child_count; i++) TraverseAST(ast_node->children[i], symbol_table);
}

// Function to check if Map is part of symboltable
void checkMap(SymbolTable table, const ASTNode* node)
{
    const SymbolTableEntry* val = LookUpSymbolTable(table, node->children[0]->data);
    if (val != NULL) //if already part of symbol table - report semantic error
        if (val->ast_location->type == AT_MAP)
            reportSemanticError(ERR_SEMANTIC, node->pos, "Map is already declared");
        else
        {
            reportSemanticError(ERR_SEMANTIC, node->pos, "Map name already declared for another type");
        }
}

// Function to check if Room is part of symboltalbe
void checkRoom(SymbolTable table, const ASTNode* node)
{
    const SymbolTableEntry* val = LookUpSymbolTable(table, node->children[0]->data);
    if (val != NULL) //if already part of symbol table - report semantic error
        if (val->ast_location->type == AT_ROOM)
        {
            reportSemanticError(ERR_SEMANTIC, node->pos, "Room is already declared");
        }
        else
        {
            reportSemanticError(ERR_SEMANTIC, node->pos, "Room name already declared for another type");
        }
}

// Function to check if Connection is part of symboltalbe
void checkConnection(SymbolTable table, const ASTNode* node)
{

    //lookup rooms in symboltable
    const SymbolTableEntry* first_room = LookUpSymbolTable(table, node->children[0]->data);
    const SymbolTableEntry* secound_room = LookUpSymbolTable(table, node->children[2]->data);

    //check for undeclared rooms in connection 
    if (!first_room || !secound_room) 
        reportSemanticError(ERR_SEMANTIC, node->pos, "Cant connect undeclared Room reference");

    //check if connection is already in symbol table
    if (LookUpConnectSymbolTable (table, first_room->ast_location->children[0]->data, secound_room->ast_location->children[0]->data)) 
        reportSemanticError(ERR_SEMANTIC, node->pos, "Connection already declared");

    //check if Rooms are of correct type
    if (first_room->ast_location->type != AT_ROOM || secound_room->ast_location->type != AT_ROOM) 
        reportSemanticError(ERR_TYPE, node->pos, "Expected Room types");

    //check if rooms are the same
    if (strcmp(node->children[0]->data, node->children[2]->data) == 0) 
        reportSemanticError(ERR_SEMANTIC, node->pos, "Can't connect a Room to itself");

}

// Error message function
void reportSemanticError(ErrorCode err, int pos, const char* msg)
{
    const char* message[] = {msg};
    reportError(err, pos, message, 1);
}

// Function to print the symboltable
void PrintSymbolTable(const SymbolTable table)
{
    if (!table)
        return;

    printf("\n Symbol Table (%zu entries):\n", table->count); 
    for (size_t i = 0; i < table->count; i++)
    {
        SymbolTableEntry entry = table->entries[i];
        printf("  Type: %s, AST location: %p \n", entry.ast_location->data, (void*)entry.ast_location);
    }
}
