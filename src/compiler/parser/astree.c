#include "astree.h"
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printNode(ASTNode* node, int depth);
void ASTAddChild(ASTNode* parentNode, ASTNode* child);
void ASTResizeChildren(ASTNode* parentNode);
ASTNode* ASTCreateTerminalNode(TokenDef op);

// Function to initialize an ASTree with a root node
ASTree ASTInit()
{
    // Allocate memory for the ASTree structure (note that ASTree is a pointer)
    ASTree tree = calloc(1, sizeof(*tree)); // Allocate a pointer to ASTree
    if (tree == NULL)
        return NULL;

    // Assign the root node to the tree's head
    tree->head = NULL; // Here we assign the root pointer to tree->head

    return tree; // Return the ASTree pointer
}

ASTNode* ASTCreateMap(ASTree tree, const char* id)
{
    ASTNode* mapNode        = calloc(1, sizeof(ASTNode));
    mapNode->type           = T_MAP;
    mapNode->child_capacity = 0;
    mapNode->child_count    = 0;
    ASTResizeChildren(mapNode);

    ASTAddChild(mapNode, ASTCreateIdentifier(id));

    return mapNode;
}

void ASTCreateRoom(ASTNode* mapNode, const char* id)
{
    if (!mapNode)
    {
        return;
    }

    if (mapNode->child_count >= mapNode->child_capacity)
    {
        ASTResizeChildren(mapNode);
    }

    ASTNode* roomNode = calloc(1, sizeof(ASTNode));
    if (roomNode == NULL)
    {
        return;
    }

    roomNode->type = T_ROOM;
    strlcpy(roomNode->data.stringVal, "Room", MAX_INPUT_SIZE - 1);

    int child_capacity       = 1;
    roomNode->child_count    = 0;
    roomNode->child_capacity = 1;
    roomNode->children       = calloc(child_capacity, sizeof(ASTNode*));

    if (roomNode->children == NULL)
    {
        free(roomNode); // Free room node if allocation fails
        return;
    }

    ASTAddChild(roomNode, ASTCreateIdentifier(id));
    ASTAddChild(mapNode, roomNode);
}

void ASTCreateConnect(ASTNode* mapNode, const char* id, const TokenDef op, const char* id2)
{

    if (mapNode == NULL)
    {
        return;
    }

    if (mapNode->child_count >= mapNode->child_capacity)
    {

        ASTResizeChildren(mapNode);
    }

    ASTNode* connectNode        = calloc(1, sizeof(ASTNode));
    connectNode->type           = T_CONNECT;
    int new_child_capacity      = 3;
    connectNode->child_count    = 0;
    connectNode->child_capacity = new_child_capacity;
    connectNode->children       = calloc(new_child_capacity, sizeof(ASTNode*));

    if (connectNode->children == NULL)
    {
        free(connectNode);
        return;
    }

    // Add children to the Connect Structure
    ASTAddChild(connectNode, ASTCreateIdentifier(id));
    ASTAddChild(connectNode, ASTCreateTerminalNode(op));
    ASTAddChild(connectNode, ASTCreateIdentifier(id2));

    ASTAddChild(mapNode, connectNode);
}

// Create an Identifier leaf node in the AST
ASTNode* ASTCreateIdentifier(const char* value)
{
    // Allocate memory for the new AST node (initialize to 0)
    ASTNode* idNode = calloc(1, sizeof(ASTNode));

    if (idNode == NULL)
    {
        // Handle memory allocation failure if needed
        free(idNode);
        return NULL;
    }

    // Ensure the string fits into the fixed-size array (truncate if needed)
    strlcpy(idNode->data.stringVal, value, MAX_INPUT_SIZE - 1);

    idNode->child_count    = 0;
    idNode->child_capacity = 0;
    idNode->children       = NULL;
    idNode->type           = T_IDENTIFIER;

    return idNode;
}

ASTNode* ASTCreateTerminalNode(TokenDef op)
{
    // Allocate memory for the new AST node (initialize to 0)
    ASTNode* terminalNode = calloc(1, sizeof(ASTNode));

    if (terminalNode == NULL)
    {
        // Handle memory allocation failure if needed
        return NULL;
    }

    terminalNode->type           = op;
    terminalNode->child_count    = 0;
    terminalNode->children       = NULL;
    terminalNode->child_capacity = 0;

    return terminalNode;
}

void ASTAddChild(ASTNode* parentNode, ASTNode* child)
{
    if (!parentNode)
        return;

    if (!child)
    {
        return;
    }

    if (parentNode->child_count >= parentNode->child_capacity)
    {
        printf("Not enough capacity to add a child.\n");
        return;
    }
    parentNode->children[parentNode->child_count] = child;
    parentNode->child_count += 1;
}

void ASTResizeChildren(ASTNode* parentNode)
{
    int new_capacity       = (parentNode->child_capacity == 0) ? 2 : parentNode->child_capacity * 1.5;
    ASTNode** new_children = realloc(parentNode->children, new_capacity * sizeof(ASTNode*));

    if (new_children == NULL)
    {
        printf("Failed to resize children array!\n");
        return;
    }

    parentNode->children       = new_children;
    parentNode->child_capacity = new_capacity; // Update capacity
}

void ASTreePrintNode(ASTNode* node, int indent)
{
    if (!node)
        return;

    // Print indentation

    // Print node type and value
    switch (node->type)
    {
        case T_MAP: printf("Map\n"); break;
        case T_ROOM: printf("\n    Room\n"); break;
        case T_CONNECT: printf("\n    Connect%s\n", node->data.stringVal); break;
        case T_DIRECTED_EDGE: printf("        ->\n"); break;
        case T_BIDIRECTIONAL_EDGE: printf("        <->\n"); break;
        case T_IDENTIFIER: printf("        %s\n", node->data.stringVal); break;
        default: break;
    }

    // Recursively print children with correct indentation
    for (int i = 0; i < node->child_count; i++)
    {
        ASTreePrintNode(node->children[i], indent);
    }
}

// Print the entire AST
void ASTreePrint(ASTree tree)
{
    if (!tree || !tree->head)
        return;
    ASTreePrintNode(tree->head, 0);
}

void ASTFreeNode(ASTNode* node)
{
    if (!node)
        return;

    // Recursively free child nodes
    for (int i = 0; i < node->child_count; i++)
    {
        ASTFreeNode(node->children[i]);
    }

    // Free the children array
    free(node->children);

    // Free the node itself
    free(node);
}

// Free the entire AST
void ASTFree(ASTree tree)
{
    if (!tree)
        return;

    // Free all nodes starting from the root
    ASTFreeNode(tree->head);

    // Free the tree structure itself
    free(tree);
}
