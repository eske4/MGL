#include "il.h"
#include "astree.h"
#include "instr_table.h"

void handle_map_node(ASTNode *node, InstructionTable table);
void handle_room_node(ASTNode *node, InstructionTable table);
void handle_connect_node(ASTNode *node, InstructionTable table);

void generate_il(ASTNode *node, InstructionTable table);


InstructionTable compile_to_il(ASTree tree){
    InstructionTable table = instr_table_init();
    generate_il(tree->head, table);
    return table;
}

void generate_il(ASTNode *node, InstructionTable table){
    if(!node) return;

    // Process different AST node types
    switch(node->type){
        case AT_MAP:
            handle_map_node(node, table);
            break;
            
        case AT_ROOM:
            handle_room_node(node, table);
            break;

        case AT_CONNECT:
            handle_connect_node(node, table);
            break;
    }

    // Recursively traverse all child nodes
    for(int i = 0; i < node->child_count; i++){
        generate_il(node->children[i], table);
    }
}

void handle_map_node(ASTNode *node, InstructionTable table) {
    if (node->child_count < 1) return;

    ASTNode *mapIdNode = node->children[0];

    char *mapId = node->children[0]->data;
    char *args[] = { mapId };

    Instruction instr = instr_create(IR_DECL_MAP, args, 1);
    instr_add(table, instr);
}

void handle_room_node(ASTNode *node, InstructionTable table) {
    if (node->child_count < 1) return;

    ASTNode *roomIdNode = node->children[0];

    char *roomId = node->children[0]->data;
    char *args[] = { roomId };

    Instruction instr = instr_create(IR_DECL_ROOM, args, 1);
    instr_add(table, instr);
}

void handle_connect_node(ASTNode *node, InstructionTable table) {
    if (node->child_count < 3) return;

    ASTNode *srcNode = node->children[0];
    ASTNode *opNode  = node->children[1];
    ASTNode *dstNode = node->children[2];

    char *src = srcNode->data;
    char *dst = dstNode->data;

    char *args[] = { src, dst };

    Instruction instr = instr_create(IR_DECL_CONNECT, args, 2);
    instr_add(table, instr);

    // If the connection is bidirectional (<->), add the reverse mapping.
    if (opNode->type == AT_BIDIRECTIONAL_EDGE) {
        char *args[] = {dst, src };

        Instruction instr = instr_create(IR_DECL_CONNECT, args, 2);
        instr_add(table, instr);
    }
}
