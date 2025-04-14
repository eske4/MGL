#pragma once

#include "definitions.h"

// Node structure for Abstract Syntax Tree (AST)
typedef struct ASTNode {
    AbstractTokenDef type;  // Enum type to specify node type
    int pos;
    char data[MAX_INPUT_SIZE];
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
ASTNode* ASTCreateMap(ASTree tree, const char* id, int pos);
void ASTCreateRoom(ASTNode *mapNode, const char* id, int pos);
void ASTCreateConnect(ASTNode *mapNode, const char* id, const AbstractTokenDef op, const char* id2, int pos);

ASTNode *ASTCreateIdentifier(const char* value);  // Create identifier leaf node
// ASTNode *ASTCreateFloat(const char value);  // Add when needed
// ASTNode *ASTCreateInt(const char value);  // Add when needed

// Debugging function
void ASTreePrint(ASTree tree);     // Print the AST for debugging

