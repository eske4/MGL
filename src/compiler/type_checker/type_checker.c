#include "type_checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler_state.h"

#define MAX_SYMBOLS 1024

static char* symbolTable[MAX_SYMBOLS];
static int symbolCount = 0;

static void addRoom(const char* id) {
    if (symbolCount < MAX_SYMBOLS) {
        symbolTable[symbolCount++] = strdup(id);
    }
}

static bool lookupRoom(const char* id) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i], id) == 0) {
            return true;
        }
    }
    return false;
}

static void clearSymbolTable() {
    for (int i = 0; i < symbolCount; i++) {
        free(symbolTable[i]);
    }
    symbolCount = 0;
}

static bool typeCheckNode(ASTNode* node) {
    if (!node) return true;

    if (node->type == AT_ROOM) {
        if (node->child_count > 0) {
            const char* roomName = node->children[0]->data.stringVal;
            addRoom(roomName);
        }
    }

    if (node->type == AT_CONNECT) {
        const char* from = node->children[0]->data.stringVal;
        const char* to = node->children[2]->data.stringVal;

        if (!lookupRoom(from)) {
            fprintf(stderr, "Type Error: Room '%s' not defined.\n at line %d", from, cs.line);
            return false;
        }

        if (!lookupRoom(to)) {
            fprintf(stderr, "Type Error: Room '%s' not defined.\n at line %d", to, cs.line);
            return false;
        }
    }

    for (int i = 0; i < node->child_count; i++) {
        if (!typeCheckNode(node->children[i])) {
            return false;
        }
    }

    return true;
}

bool typeCheck(ASTree tree) {
    if (!tree || !tree->head) return false;

    clearSymbolTable(); // fresh start
    bool success = typeCheckNode(tree->head);
    clearSymbolTable(); // clean up
    return success;
}
