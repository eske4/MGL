#include "astree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printNode(ASTNode *node, int depth);

// Function to initialize an ASTree with a root node
ASTree ASTInit() {
  // Allocate memory for the ASTree structure (note that ASTree is a pointer)
  ASTree tree = calloc(1, sizeof(*tree)); // Allocate a pointer to ASTree
  if (tree == NULL) { // Return NULL if memory allocation failed
    return NULL;
  }

  // Assign the root node to the tree's head
  tree->head = NULL; // Here we assign the root pointer to tree->head

  return tree; // Return the ASTree pointer
}

ASTNode *ASTCreateNode(Type type, struct ASTNode *left, struct ASTNode *right,
                       void *value) {
  struct ASTNode *n;

  // Malloc a new ASTnode
  n = calloc(1, sizeof(*n));
  if (n == NULL) {
    exit(1);
  }
  // Copy in the field values and return it
  n->type = type;
  n->left = left;
  n->right = right;

  if (value != NULL) {
    switch (type) {
    case TYPE_IDENTIFIER:
      n->value.stringVal =
          strdup((char *)value); // For identifiers, copy the string
      break;
    case TYPE_INTEGER_LITERAL:
      n->value.intValue = (int)n->value.intValue; // For integers
      break;
    case TYPE_FLOAT_LITERAL:
      n->value.floatValue = (float)n->value.intValue; // For integers

      break;
    default:
      n->value.stringVal = NULL; // Default to NULL if the value isn't handled
      break;
    }
  } else {
    n->value.stringVal = NULL; // If value is NULL, set it to NULL
  }

  return (n);
}

ASTNode *ASTCreateLeaf(Type op, void *value) {
  return ASTCreateNode(op, NULL, NULL, NULL);
}

// Helper function to free a node and its children
void ASTFreeNode(ASTNode *node) {
  if (node == NULL)
    return;

  // Recursively free left and right children first
  ASTFreeNode(node->left);
  ASTFreeNode(node->right);

  // If the node's value is a string (like identifier), free it
  if (node->value.stringVal != NULL) {
    free(node->value.stringVal);
  }

  // Finally, free the node itself
  free(node);
}

// Main function to free the ASTree (tree structure itself)
void ASTFree(ASTree tree) {
  if (tree == NULL)
    return;

  // Free the nodes starting from the head of the tree
  ASTFreeNode(tree->head);

  // After freeing the nodes, free the tree structure itself
  free(tree);

  // Set the tree pointer to NULL to avoid dangling pointers
  tree = NULL;
}

// Value leafs
ASTNode *ASTCreateIdentifier(char value[]) {
  return ASTCreateNode(TYPE_IDENTIFIER, NULL, NULL, value);
}

// Leafs for structures and operations
ASTNode *ASTCreateMap(ASTNode *left, ASTNode *right) {
  return ASTCreateNode(TYPE_MAP, left, right, NULL);
}

ASTNode *ASTCreateRoom(ASTNode *left) {
  return ASTCreateNode(TYPE_ROOM, left, NULL, NULL);
}

// Function to create an Arrow node (->) between identifiers
ASTNode *ASTCreateDirectedEdge(ASTNode *left, ASTNode *right) {
  return ASTCreateNode(TYPE_DIRECTED_EDGE, left, right, NULL);
}

ASTNode *ASTCreateBidirectionalEdge(ASTNode *left, ASTNode *right) {
  return ASTCreateNode(TYPE_BIDIRECTIONAL_EDGE, left, right, NULL);
}

ASTNode *ASTCreateExpressionChain(ASTNode *left, ASTNode *right) {
  return ASTCreateNode(TYPE_EXPRESSION_CHAIN, left, right, NULL);
}

void ASTreePrint(ASTree tree) {
  if (tree == NULL || tree->head == NULL) {
    return;
  }

  // Start the recursive traversal from the root node
  printNode(tree->head, 0);
}

// Helper function to print individual nodes recursively in a side-by-side tree
// format
void printNode(ASTNode *node, int depth) {
  if (node == NULL) {
    for (int i = 0; i < depth; i++)
      printf("    ");
    printf("└── NULL\n");
    return;
  }

  // Print indentation for hierarchy
  for (int i = 0; i < depth; i++)
    printf("│   ");

  // Print node type and details
  switch (node->type) {
  case TYPE_MAP:
    printf("MAP\n");
    break;
  case TYPE_ROOM:
    printf("ROOM\n");
    break;
  case TYPE_DIRECTED_EDGE:
    printf("DIRECTED\n");
    break;
  case TYPE_BIDIRECTIONAL_EDGE:
    printf("BIDIRECTIONAL\n");
    break;

  case TYPE_IDENTIFIER:
    printf("IDENTIFIER %s\n",
           node->value.stringVal ? node->value.stringVal : "NULL");
    return; // Identifiers are leaf nodes, so no need to recurse further
  case TYPE_EXPRESSION_CHAIN:
    break;
  default:
    printf("UNKNOWN (%d)\n", node->type); // Debugging unexpected node types
    break;
  }

  // Recursively print child nodes
  if (node->left)
    printNode(node->left, depth + 1);
  if (node->right)
    printNode(node->right, depth + 1);
}
