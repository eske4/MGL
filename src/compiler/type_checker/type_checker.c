#include "type_checker.h"
#include "error_handler.h"
#include "look_up_tables.h"
#include "definitions.h"
#include <string.h>

void checkRoom(SymbolTable table, const char *id, const ASTNode *node);
void checkConnection(char *id, char *id2, SymbolTable table, const ASTNode *node);
void reportSemanticError(ErrorCode err, int pos, const char* msg);

void TraverseAST(const ASTNode* node, const SymbolTable table){
    if(!node) return;

    switch(node->type){
        case AT_MAP:
            TTAdd(table, node->children[0]->data, node->type); break;

        case AT_ROOM:
            checkRoom(table, node->children[0]->data, node);
            TTAdd(table, node->children[0]->data, node->type); break;
        case AT_CONNECT:
            checkConnection(node->children[0]->data, node->children[2]->data, table, node);
        default: break;
    }

    for(int i = 0; i < node->child_count; i++){
        TraverseAST(node->children[i], table);
    }
}

void TypeCheck(const ASTree tree){
    SymbolTable table = TTInit(20);
    ASTNode* root = tree->head;
    TraverseAST(root, table);
    TTFree(table);
}

void checkRoom(SymbolTable table, const char *id, const ASTNode* node){
    const SymbolTableEntry *val = TTLookUp(table, id);
    if(val != NULL)
        reportSemanticError(ERR_SEMANTIC, node->pos, "Room is already declared");
}

void checkConnection(char *id, char *id2, SymbolTable table, const ASTNode* node){
    const SymbolTableEntry *idRef = TTLookUp(table, id);
    const SymbolTableEntry *id2Ref = TTLookUp(table, id2);


    if(!idRef || !id2Ref)
        reportSemanticError(ERR_SEMANTIC, node->pos, "Cant connect undeclared Room reference");

    if(idRef->token != AT_ROOM || id2Ref->token != AT_ROOM)
        reportSemanticError(ERR_TYPE, node->pos, "Expected Room types");

    if(strcmp(idRef->id, id2Ref->id) == 0)
        reportSemanticError(ERR_SEMANTIC, node->pos, "Can't connect a Room to itself");



}

void reportSemanticError(ErrorCode err, int pos, const char* msg){
    const char *message[] = {msg};
    reportError(err, pos, message, 1);
}

