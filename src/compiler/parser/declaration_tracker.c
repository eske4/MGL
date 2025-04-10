#include "declaration_tracker.h"
#include "type_table.h"
#include "definitions.h"
#include "stdio.h"
#include "type_table.h"
#include <stdlib.h>

void TraverseAST(const ASTNode* node, const TypeTable table){
    if(!node) return;

    switch(node->type){
        case AT_MAP:
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;

        case AT_ROOM:
            TTAdd(table, node->children[0]->data.stringVal, node->type); break;
        case AT_CONNECT:
            const TypeTableEntry *id = TTFind(table, node->children[0]->data.stringVal);
            const TypeTableEntry *id2 = TTFind(table, node->children[2]->data.stringVal);
            if(!id || !id2) {
                TTFree(table); 
                fprintf(stderr, "Parser Error: Connecting undeclared rooms\n"); 
                exit(1);
            }         
        default: break;
    }

    for(int i = 0; i < node->child_count; i++){
        TraverseAST(node->children[i], table);
    }
}

void AnalyseAST(const ASTree tree){
    TypeTable table = TTInit(20);
    ASTNode* root = tree->head;
    TraverseAST(root, table);
    TTFree(table);
}

