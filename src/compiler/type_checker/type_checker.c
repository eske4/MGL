#include "type_checker.h"
#include "error_handler.h"
#include "look_up_tables.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//define duble struct for constr
typedef struct {
    int first;
    int second;
} Duple;

//declaration to let program know these function/declaration exist (top-down compiled )
void checkMap(SymbolTable table, const char *id, const ASTNode *node);
void checkRoom(SymbolTable table, const char *id, const ASTNode *node);
void checkRoomsInConnection(char *id, char *id2, SymbolTable table, const ASTNode *node);
void checkConnection(SymbolTable table, const char *id, const ASTNode *node);
void checkConnectConstr(SymbolTable table, int value);
void reportSemanticError(ErrorCode err, int pos, const char* msg);
void PrintSymbolTable(const SymbolTable table);
static Duple roomConstr = {0, 0};
static Duple connectConstr = {0, 0};
static int roomCount = 0;

//function to treverse AST and add to symboltable
void TraverseAST(const ASTNode* node, const SymbolTable table){
    if(!node) return; //safty to check if node exist in AST

    switch(node->type){  //use switch to check multible cases for the type
        case AT_MAP: //if AT_MAP type
            AddSymbolTable(table, node->children[0]->data, node); break;
        case AT_MAP_CONSTR_ROOMS:
            roomConstr.first = 1; 
            roomConstr.second = atoi(node->children[0]->data); break; //rewrite to make safer conversion
        
        case AT_MAP_CONSTR_CONNECT: 
            connectConstr.first = 1;
            connectConstr.second = atoi(node->children[0]->data); break;//rewrite to make safer conversion

        case AT_ROOM: //if AT_ROOM type
            checkRoom(table, node->children[0]->data, node);
            AddSymbolTable(table, node->children[0]->data, node); 
            if (roomConstr.first == 1){
                roomCount ++;
                if (roomCount > roomConstr.second) 
                    reportSemanticError(ERR_SEMANTIC, 0, "Number of rooms exceeds room constraint"); // need better error message
                break;
            }
            
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

//main typecheck function
void TypeCheck(const ASTree tree)
{
    SymbolTable table = InitSymbolTable(20); //initiate symboltable with a initial capacity of 20
    ASTNode* root = tree->head;
    TraverseAST(root, table); 
    if (connectConstr.first == 1)
        checkConnectConstr(table, connectConstr.second);
    PrintSymbolTable(table); 
    FreeSymbolTable(table);
}

//ceck if map is part of symboltalbe
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

void checkConnectConstr(SymbolTable table, int value){

}
//error message function 
void reportSemanticError(ErrorCode err, int pos, const char* msg){
    const char *message[] = {msg};
    reportError(err, pos, message, 1);
}

//function to print the symboltable
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
