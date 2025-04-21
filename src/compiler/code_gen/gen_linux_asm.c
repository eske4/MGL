#include "code_gen.h"
#include "instr_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Room {
    char *id;
    char **connections;
    int connCount;
} Room;

void getConnections(InstructionTable table, Room *room, int startIndex);
void writeConnectAssembly(Room *room, FILE *file);
void writeRoomAssembly(Room *room, FILE *file);


void generate_assembly(InstructionTable table) {
    FILE *file = fopen("map.asm", "w");

    if(table->count >= 1){
        fprintf(file, "section .data\n\n");

    }
    
    for (int i = 0; i < table->count; i++) {
        if (table->entries[i].InstrCode != IR_DECL_ROOM) continue;

        // Allocate and initialize Room
        Room *currentRoom = malloc(sizeof(Room));
        currentRoom->id = table->entries[i].args[0];
        currentRoom->connCount = 0;

        // Allocate memory for connections
        currentRoom->connections = calloc(MAX_CONNECTIONS, sizeof(char*));

        if (!currentRoom->connections) {
            perror("Memory allocation failed for connections\n");
            free(currentRoom);
            return;
        }

        getConnections(table, currentRoom, i + 1);
        writeRoomAssembly(currentRoom, file);
        writeConnectAssembly(currentRoom, file);

        free(currentRoom->connections);
        free(currentRoom);
    }
    fclose(file);
}

void getConnections(InstructionTable table, Room *room, int startIndex) {
    for (int i = startIndex; i < table->count; i++) {
        if (table->entries[i].InstrCode != IR_DECL_CONNECT)
            continue;

        // Use strcmp for string comparison
        if (strcmp(table->entries[i].args[0], room->id) != 0)
            continue;

        if (room->connCount >= MAX_CONNECTIONS) {
            fprintf(stderr, "Too many connections for room %s\n", room->id);
            break;
        }

        room->connections[room->connCount++] = table->entries[i].args[1];
    }
}

void writeRoomAssembly(Room *room, FILE *file){
    fprintf(file, "\n");
    if(room == NULL || room->id == NULL) return;
    fprintf(file, "%s:\n", room->id);
    fprintf(file, "    db \"%s\", 0\n", room->id);
}
void writeConnectAssembly(Room *room, FILE *file) {
    if(room == NULL || room->connCount < 1) return;

    fprintf(file, "    dq ");

    for (int i = 0; i < room->connCount; i++) {
        fprintf(file, "%s", room->connections[i]);
        if (i < room->connCount - 1) {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ", 0\n\n");

}
