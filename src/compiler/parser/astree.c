#include "astree.h"
#include "string_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printNode(ASTNode* node, int depth);
void ASTAddChild(ASTNode* parentNode, ASTNode* child);
void ASTResizeChildren(ASTNode* parentNode);
ASTNode* ASTCreateTerminalNode(AbstractTokenDef op);

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

ASTNode* ASTCreateMap(ASTree tree, const char* id, int pos)
{
    ASTNode* mapNode        = calloc(1, sizeof(ASTNode));
    mapNode->type           = AT_MAP;
    mapNode->pos            = pos;
    safe_strcpy(mapNode->data, "Map", MAX_INPUT_SIZE - 1);
    mapNode->child_capacity = 0;
    mapNode->child_count    = 0;
    ASTResizeChildren(mapNode);

    ASTAddChild(mapNode, ASTCreateIdentifier(id));
    return mapNode;
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
    safe_strcpy(idNode->data, value, MAX_INPUT_SIZE - 1);

    idNode->child_count    = 0;
    idNode->child_capacity = 0;
    idNode->children       = NULL;
    idNode->type           = AT_IDENTIFIER;

    return idNode;
}

ASTNode* ASTCreateConstrValue(const char* value)
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
    safe_strcpy(idNode->data, value, MAX_INPUT_SIZE - 1);

    idNode->child_count    = 0;
    idNode->child_capacity = 0;
    idNode->children       = NULL;
    idNode->type           = AT_CONSTR_VALUE;

    return idNode;
}

ASTNode* ASTCreateTerminalNode(AbstractTokenDef op)
{
    // Allocate memory for the new AST node (initialize to 0)
    ASTNode* terminalNode = calloc(1, sizeof(ASTNode));

    if (terminalNode == NULL)
        return NULL;

    terminalNode->type           = op;
    terminalNode->child_count    = 0;
    terminalNode->children       = NULL;
    terminalNode->child_capacity = 0;

    return terminalNode;
}

void ASTCreateMapConstr(ASTNode *mapNode, const char* id, int pos, AbstractTokenDef type, const char* value){
    //check if given mapNode is not null
    if(!mapNode)
        return; 
    //check if given mapNode need more capasity for child nodes
    if (mapNode->child_count >= mapNode->child_capacity)
        ASTResizeChildren(mapNode);

    //reserve memory for a map constraint node
    ASTNode* mapConstrNode = calloc(1, sizeof(ASTNode));
    
    //check if resevation/allocation worked
    if (mapConstrNode == NULL)
        return; 

    //check if constraint type is given and set mapCostrNode type to given type
    if (type)
        mapConstrNode->type = type;

    //add id to mapConstrNode
    if (id)
        safe_strcpy(mapConstrNode->data, id, MAX_INPUT_SIZE - 1); 

    //add data to mapConstrNode
    int child_capacity              = 1;
    mapConstrNode->pos              = pos; 
    mapConstrNode->child_count      = 0;
    mapConstrNode->child_capacity   = 1;
    mapConstrNode->children         = calloc(child_capacity, sizeof(ASTNode*));

    //free mapConstrNode if allocation did not work 
    if (mapConstrNode->children == NULL){
        free(mapConstrNode);
        return; 
    }

    //add child ASTNode to the mapConstrNode 
    ASTAddChild(mapConstrNode, ASTCreateConstrValue(value));

    //add the mapContrNode as a child to Map 
    ASTAddChild(mapNode, mapConstrNode);

}

void ASTCreateRoom(ASTNode* mapNode, const char* id, int pos)
{
    if (!mapNode)
        return;

    if (mapNode->child_count >= mapNode->child_capacity)
        ASTResizeChildren(mapNode);

    ASTNode* roomNode = calloc(1, sizeof(ASTNode));
    if (roomNode == NULL)
        return;

    roomNode->type = AT_ROOM;
    safe_strcpy(roomNode->data, "Room", MAX_INPUT_SIZE - 1);

    int child_capacity       = 1;
    roomNode->pos            = pos;
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

void ASTCreateConnect(ASTNode* mapNode, const char* id, const AbstractTokenDef op, const char* id2, int pos)
{

    if (mapNode == NULL)
        return;

    if (mapNode->child_count >= mapNode->child_capacity)
        ASTResizeChildren(mapNode);

    ASTNode* connectNode        = calloc(1, sizeof(ASTNode));
    connectNode->type           = AT_CONNECT;
    connectNode->pos            = pos;
    safe_strcpy(connectNode->data, "Connect", MAX_INPUT_SIZE - 1);
    connectNode->child_count    = 0;
    connectNode->child_capacity = 3;
    connectNode->children       = calloc(3, sizeof(ASTNode*));

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

void ASTAddChild(ASTNode* parentNode, ASTNode* child)
{
    if (!parentNode || !child)
        return;

    if (parentNode->child_count >= parentNode->child_capacity)
    {
        perror("Not enough capacity to add a child.\n");
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
        perror("Failed to resize children array!\n");
        return;
    }

    parentNode->children       = new_children;
    parentNode->child_capacity = new_capacity; // Update capacity
}

//=================print AST function=================

void ASTreePrintNode(ASTNode* node, int indent)
{
    if (!node)
        return;

    // Print indentation

    // Print node type and value
    switch (node->type)
    {
        case AT_MAP: printf("\nMap\n"); break;
        case AT_ROOM: printf("\n    Room\n"); break;
        case AT_CONNECT: printf("\n    Connect%s\n", node->data); break;
        case AT_DIRECTED_EDGE: printf("        ->\n"); break;
        case AT_BIDIRECTIONAL_EDGE: printf("        <->\n"); break;
        case AT_IDENTIFIER: printf("        %s\n", node->data); break;
        case AT_MAP_CONSTR_ROOMS: printf("        %s\n", node->data); break;
        case AT_MAP_CONSTR_CONNECT: printf("        %s\n", node->data); break;
        case AT_CONSTR_VALUE: printf("            %s\n", node->data); break;
        default: printf("Unknown node type: %d\n", node->type); break; //print in AST if a node exist with different type
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
    printf("\nAbstract Syntax Tree:");
    ASTreePrintNode(tree->head, 0);
}

//=================free AST functions=================

void ASTFreeNode(ASTNode* node)
{
    if (!node)
        return;

    // Recursively free child nodes
    for (int i = 0; i < node->child_count; i++)
        ASTFreeNode(node->children[i]);

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
