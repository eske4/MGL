#include "ir_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize a dynamic array for IR instructions.
void initIRArray(IRArray* array) {
    array->capacity = 10;
    array->count = 0;
    array->instructions = (IRInstruction*) malloc(array->capacity * sizeof(IRInstruction));
    if (array->instructions == NULL) {
        fprintf(stderr, "Failed to allocate memory for IR instructions.\n");
        exit(EXIT_FAILURE);
    }
}

// Append an IR instruction to the dynamic array.
void appendIR(IRArray* array, IRInstruction instr) {
    if (array->count >= array->capacity) {
        array->capacity *= 2;
        IRInstruction* new_instructions = (IRInstruction*) realloc(array->instructions, array->capacity * sizeof(IRInstruction));
        if (new_instructions == NULL) {
            fprintf(stderr, "Failed to resize IR instructions array.\n");
            exit(EXIT_FAILURE);
        }
        array->instructions = new_instructions;
    }
    array->instructions[array->count++] = instr;
}

// Free the memory allocated for the IR array.
void freeIRArray(IRArray* array) {
    free(array->instructions);
    array->instructions = NULL;
    array->count = 0;
    array->capacity = 0;
}

void compileToIR(ASTree tree, IRArray* outArray) {
    initIRArray(outArray);
    generateIR(tree->head, outArray);
}

void generateIR(ASTNode* node, IRArray* array) {
    if (!node) return;

    switch (node->type) {
        case AT_MAP:
            handleMapNode(node, array);
            break;
        case AT_ROOM:
            handleRoomNode(node, array);
            break;
        case AT_CONNECT:
            handleConnectNode(node, array);
            break;
        default:
            fprintf(stderr, "[IR ERROR] Unhandled AST node type: %d\n", node->type);
            exit(EXIT_FAILURE);
    }
}

void handleMapNode(ASTNode* node, IRArray* array) {
    IRInstruction instr;
    instr.type = IR_DECL_MAP;

    ASTNode* mapIdNode = node->children[0];
    strcpy(instr.data.mapDecl.mapName, mapIdNode->data.stringVal);
    appendIR(array, instr);

    for (int i = 1; i < node->child_count; i++) {
        generateIR(node->children[i], array);
    }
}

void handleRoomNode(ASTNode* node, IRArray* array) {
    IRInstruction instr;
    instr.type = IR_DECL_ROOM;

    ASTNode* roomIdNode = node->children[0];
    strcpy(instr.data.roomDecl.roomName, roomIdNode->data.stringVal);
    appendIR(array, instr);
}

void handleConnectNode(ASTNode* node, IRArray* array) {
    IRInstruction instr;
    instr.type = IR_CREATE_CONNECTION;

    ASTNode* srcNode = node->children[0];
    ASTNode* opNode  = node->children[1];
    ASTNode* dstNode = node->children[2];

    const char* src = srcNode->data.stringVal;
    const char* dst = dstNode->data.stringVal;

    // First connection
    strcpy(instr.data.connectDecl.src, src);
    strcpy(instr.data.connectDecl.dst, dst);
    appendIR(array, instr);

    // If the connection is bidirectional (<->), add the reverse mapping.
    if (opNode->type == AT_BIDIRECTIONAL_EDGE) {
        strcpy(instr.data.connectDecl.src, dst);
        strcpy(instr.data.connectDecl.dst, src);
        appendIR(array, instr);
    }
}


// Print out the IR instructions in a human-readable format.
void printIR(const IRArray* array) {
    size_t i;
    printf("\nIntermediate Representation (IR):\n");
    for (i = 0; i < array->count; i++) {
        IRInstruction instr = array->instructions[i];
        switch (instr.type) {
            case IR_DECL_MAP:
                printf("IR_DECL_MAP: %s\n", instr.data.mapDecl.mapName);
                break;
            case IR_DECL_ROOM:
                printf("IR_DECL_ROOM: %s\n", instr.data.roomDecl.roomName);
                break;
            case IR_CREATE_CONNECTION:
                printf("IR_CREATE_CONNECTION: %s -> %s\n", 
                       instr.data.connectDecl.src, 
                       instr.data.connectDecl.dst);
                break;
            default:
                printf("Unknown IR Instruction\n");
                break;
        }
    }
}