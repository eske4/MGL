#pragma once

#include "astree.h" 
#include <stddef.h>
#include "definitions.h"

// Define IR instruction types.
typedef enum {
    IR_DECL_MAP,          // Declares a map.
    IR_DECL_ROOM,         // Declares a room.
    IR_CREATE_CONNECTION  // Creates a connection between rooms.
} IRInstructionType;

// Define IR connection types. Maybe don't need for IR?
// typedef enum {
//     IR_DIRECTED_EDGE, // ->
//     IR_BIDIRECTIONAL_EDGE // <->
// } IRConnectionType;

typedef struct {
    IRInstructionType type;
    union {
        struct {
            char mapName[MAX_INPUT_SIZE];
        } mapDecl;
        struct {
            char roomName[MAX_INPUT_SIZE];
        } roomDecl;
        struct {
            char src[MAX_INPUT_SIZE];
            char dst[MAX_INPUT_SIZE];
            // IRConnectionType direction;
        } connectDecl;
    } data;
} IRInstruction;


// Define a dynamic array for IR instructions.
typedef struct {
    IRInstruction* instructions;
    size_t count;
    size_t capacity;
} IRArray;

void initIRArray(IRArray* array); // Initializes a dynamic IR instruction array with default capacity.
void appendIR(IRArray* array, IRInstruction instr); // Appends a new IR instruction to the instruction array.
void freeIRArray(IRArray* array); // Frees memory allocated for the IR instruction array.
void compileToIR(ASTree tree, IRArray* outArray); // Compiles AST to IR
void generateIR(ASTNode* node, IRArray* array); // Generates intermediate representation (IR) from an AST node.
void handleMapNode(ASTNode* node, IRArray* array); // Handles IR generation for an AT_MAP node and its children.
void handleRoomNode(ASTNode* node, IRArray* array); // Handles IR generation for an AT_ROOM node.
void handleConnectNode(ASTNode* node, IRArray* array); // Handles IR generation for an AT_CONNECT node.
void printIR(const IRArray* array); // Prints the IR instruction list in a human-readable format (for debugging).
