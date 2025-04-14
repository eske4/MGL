#include "type_checker.h"
#include "look_up_tables.h"
#include "definitions.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

void checkRoom(SymbolTable table, const char *id);
void checkConnection(char *id, char *id2, SymbolTable table);

void TraverseAST(const ASTNode* node, const SymbolTable table){
    if(!node) return;

    switch(node->type){
        case AT_MAP:
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;

        case AT_ROOM:
            checkRoom(table, node->children[0]->data.stringVal);
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;
        case AT_CONNECT:
            checkConnection(node->children[0]->data.stringVal, node->children[2]->data.stringVal, table);
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

void checkRoom(SymbolTable table, const char *id){
    const SymbolTableEntry *val = TTLookUp(table, id);
    if(val != NULL){
        fprintf(stderr, "Type error: Can't redeclare room!\n");
        exit(1);
    }
}

void checkConnection(char *id, char *id2, SymbolTable table){
    const SymbolTableEntry *idRef = TTLookUp(table, id);
    const SymbolTableEntry *id2Ref = TTLookUp(table, id2);


    if(!idRef || !id2Ref){
        fprintf(stderr, "Type error: Connecting undeclared rooms\n"); 
        exit(1);
    }

    if(idRef->token != AT_ROOM || id2Ref->token != AT_ROOM){
        fprintf(stderr, "%d %d", idRef->token, id2Ref->token);
        fprintf(stderr, "Type error: Expected a Room type but got something else\n");
        exit(1);
    }

    if(strcmp(idRef->id, id2Ref->id) == 0){
        fprintf(stderr, "Type error: Rooms can't connect to themselves\n");
        exit(1);

    }


}
