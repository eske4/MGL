#include "il.h"
#include "astree.h"
#include "instr_table.h"

void astToInstr(ASTNode *node, InstructionTable table);

InstructionTable initIT(ASTree tree){
    // Initialize the instruction table
    InstructionTable table = InstrInit();
    astToInstr(tree->head, table);
    return table;
}

void astToInstr(ASTNode *node, InstructionTable table){
    if(!node) return; // Check for null node

    // Declare the Instruction variable once here, outside of the switch-case
    Instruction instr;

    // Process different AST node types
    switch(node->type){
        case AT_MAP:
            // Ensure the first child has a valid string value
            if (node->children[0] && node->children[0]->data) {
                char *args[] = { node->children[0]->data };
                instr = InstrMake(IR_DECL_MAP, args, 1);
                InstrAdd(table, instr);
            }
            break;
            
        case AT_ROOM:
            // Ensure the first child has a valid string value
            if (node->children[0] && node->children[0]->data) {
                char *args[] = { node->children[0]->data };
                instr = InstrMake(IR_DECL_ROOM, args, 1);
                InstrAdd(table, instr);
            }
            break;

        case AT_CONNECT:
            // Ensure the first and third children have valid string values
            if (node->children[0] && node->children[0]->data && 
                node->children[2] && node->children[2]->data) {
                char *args[] = { node->children[0]->data, node->children[2]->data };
                instr = InstrMake(IR_DECL_CONNECT, args, 2);
                InstrAdd(table, instr);
            }
            break;
    }

    // Recursively traverse all child nodes
    for(int i = 0; i < node->child_count; i++){
        astToInstr(node->children[i], table);
    }
}


