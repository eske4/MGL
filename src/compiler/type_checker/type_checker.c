#include "type_checker.h"
#include "compiler_state.h"
#include "look_up_tables.h"
#include "definitions.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

void checkRoom(SymbolTable table, const char *id, const ASTNode *node);
void checkConnection(char *id, char *id2, SymbolTable table, const ASTNode *node);
void reportError(const ASTNode *node);

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
    if(val != NULL){
        reportError(node);
    }
}

void checkConnection(char *id, char *id2, SymbolTable table, const ASTNode* node){
    const SymbolTableEntry *idRef = TTLookUp(table, id);
    const SymbolTableEntry *id2Ref = TTLookUp(table, id2);


    if(!idRef || !id2Ref){
        reportError(node);
        fprintf(stderr, "Type error: Connecting undeclared rooms\n"); 
        exit(1);
    }

    if(idRef->token != AT_ROOM || id2Ref->token != AT_ROOM){
        reportError(node);
        fprintf(stderr, "%d %d", idRef->token, id2Ref->token);
        fprintf(stderr, "Type error: Expected a Room type but got something else\n");
        exit(1);
    }

    if(strcmp(idRef->id, id2Ref->id) == 0){
        reportError(node);

    }


}

void reportError(const ASTNode *node){
    CLoc loc = findLoc(node->pos);
    fprintf(stderr, "Type error at line %d in column %d", loc.line, loc.column);
    exit(1);
}

