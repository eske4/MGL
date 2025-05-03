#include "type_checker.h"
#include "error_handler.h"
#include "look_up_tables.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//declaration to let program know these function exist (top-down compiled )
void checkRoom(SymbolTable table, const char* id, const ASTNode* node);
void checkRoomsInConnection(char* id, char* id2, SymbolTable table, const ASTNode* node);
void checkConnection(SymbolTable table, const char* id, const ASTNode* node);
void reportSemanticError(ErrorCode err, int pos, const char* msg);

//function to
void TraverseAST(const ASTNode* node, const SymbolTable table)
{
    if (!node)
        return; //safty to check if node exist in AST

    switch (node->type)
    {                //use switch to check multible cases for the type
        case AT_MAP: //if AT_MAP type
            AddSymbolTable(table, node->children[0]->data, node);
            break;

        case AT_ROOM: //if AT_ROOM type
            checkRoom(table, node->children[0]->data, node);
            AddSymbolTable(table, node->children[0]->data, node);
            break;

        case AT_CONNECT: //if AT_ROOM type
        { 
            char* connection_id = calloc(1, strlen(node->children[0]->data) + strlen(node->children[2]->data) + 2);
            sprintf(connection_id, "#%s%s", node->children[0]->data, node->children[2]->data);

            checkConnection(table, connection_id, node);
            checkRoomsInConnection(node->children[0]->data, node->children[2]->data, table, node);
            AddSymbolTable(table, connection_id, node);
        }
        default: break;
        
    }

    for (int i = 0; i < node->child_count; i++)
    { //recursively go throug each node in AST
        TraverseAST(node->children[i], table);
    }
}

//function to print the symboltable
void PrintSymbolTable(const SymbolTable table)
{
    if (!table)
        return;

    printf("\n Symbol Table (%zu entries):\n", table->count);
    for (size_t i = 0; i < table->count; i++)
    {
        SymbolTableEntry entry = table->entries[i];
        printf("  ID: %s, AST location: %p \n", entry.id, (void*)entry.ast_location);
    }
}

//main typecheck function
void TypeCheck(const ASTree tree)
{
    SymbolTable table = InitSymbolTable(20); //initiate symboltable with a initial capacity of 20
    ASTNode* root     = tree->head;
    TraverseAST(root, table);
    PrintSymbolTable(table);
    FreeSymbolTable(table);
}

//ceck if room is part of symboltalbe
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

//ceck if connection is part of symboltalbe
void checkConnection(SymbolTable table, const char* id, const ASTNode* node)
{
    const SymbolTableEntry* val = LookUpSymbolTable(table, id);
    if (val != NULL) //if already part of symbol table - report semantic error
        reportSemanticError(ERR_SEMANTIC, node->pos, "Connection is already declared");
}

//check for for rooms in connection
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

//error message function
void reportSemanticError(ErrorCode err, int pos, const char* msg)
{
    const char* message[] = {msg};
    reportError(err, pos, message, 1);
}
