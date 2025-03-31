#pragma once

#include "definitions.h"

typedef struct ASTNode {
  Type type;
  union {
    char *stringVal;
    int intValue;
    float floatValue;
  } value;
  struct ASTNode *left;
  struct ASTNode *right;

} ASTNode;

typedef struct ASTreeStruct {
  ASTNode *head;
} *ASTree;

// Basic creation of Abstract syntax tree
ASTree ASTInit(); // Initialize tree
ASTNode *ASTCreateNode(Type type, ASTNode *left, ASTNode *right, void *value);
ASTNode *ASTCreateLeaf(Type type, void *value); // Build a leaf
void ASTFree(ASTree tree);                      // free memory / delete
void ASTreePrint(ASTree tree);

// Leaf nodes (simple value nodes)
ASTNode *ASTCreateIdentifier(char *identifiers);

// Non-leaf nodes (complex nodes representing structures or operations)
ASTNode *ASTCreateExpressionChain(ASTNode *left, ASTNode *right);
ASTNode *ASTCreateRoom(ASTNode *left);
ASTNode *ASTCreateMap(ASTNode *left, ASTNode *right);
ASTNode *ASTCreateDirectedEdge(ASTNode *left, ASTNode *right);
ASTNode *ASTCreateBidirectionalEdge(ASTNode *left, ASTNode *right);
