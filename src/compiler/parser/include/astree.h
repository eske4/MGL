#pragma once

#include "definitions.h"

// Node structure for Abstract Syntax Tree (AST)
typedef struct ASTNode {
    TokenDef type;  // Enum type to specify node type

    union {
        char stringVal[MAX_INPUT_SIZE];
        int intValue;
        float floatValue;
    } data;
    struct ASTNode** children;  // Array of child nodes (NULL-terminated or dynamic)
    int child_count; // To manage children location so we fx don't overide id1 with id2 and result in only 1 id
    int child_capacity;
} ASTNode;

// Tree structure holding the root of the AST
typedef struct ASTreeStruct {
    ASTNode *head;  // Root of the tree
} *ASTree;

// Function prototypes for AST creation, deletion, and printing

ASTree ASTInit();                  // Initialize a new tree
void ASTFree(ASTree tree);         // Free all tree nodes and memory

// Node creation functions for different types of nodes
ASTNode* ASTCreateMap(ASTree tree, const char* id);
void ASTCreateRoom(ASTNode *mapNode, const char* id);
void ASTCreateConnect(ASTNode *mapNode, const char* id, const TokenDef op, const char* id2);

ASTNode *ASTCreateIdentifier(const char* value);  // Create identifier leaf node
// ASTNode *ASTCreateFloat(const char value);  // Add when needed
// ASTNode *ASTCreateInt(const char value);  // Add when needed

// Debugging function
void ASTreePrint(ASTree tree);     // Print the AST for debugging

