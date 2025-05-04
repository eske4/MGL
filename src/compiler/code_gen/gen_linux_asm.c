#include "code_gen.h"
#include "definitions.h"
#include "instr_table.h"
#include "file_config.c"
#include "string_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_map(InstructionTable table, const char* path[]);
void generate_config(InstructionTable table);
void generate_interfaces(InstructionTable table);
int countRooms(InstructionTable table);
char** getRoomNames(InstructionTable table);
void getConnections(InstructionTable table, Room* room, int startIndex);
void writeConnectAssembly(Room* room, FILE* file);
void writeRoomAssembly(Room* room, FILE* file);
void writeCInterface(char** rooms, int roomCount, int roomNameSize, int maxConnections);

void generate_assembly(InstructionTable table)
{
    printf("Code Generation\n");

    const char* asmVerificationPath[] = {ASM_DIR, "map.asm"};
    const char* generatedPath[]       = {GENERATED_DIR, "asm/map.asm"};

    printf("\n");
    printf("Generating map...\n");
    generate_map(table, asmVerificationPath);
    generate_map(table, generatedPath);
    printf("SUCCESSFULLY: generated map\n\n");

    printf("Generating interfaces...\n");
    generate_interfaces(table);
    printf("SUCCESSFULLY: generated interfaces\n\n");

    printf("Generating configs...\n");
    generate_config(table);
    printf("SUCCESSFULLY: generated configs\n\n");
}

void generate_interfaces(InstructionTable table)
{
    int roomCount = countRooms(table);
    char** rooms  = getRoomNames(table);
    writeCInterface(rooms, roomCount, MAX_INPUT_SIZE, MAX_CONNECTIONS);
    free(rooms);
}

void generate_config(InstructionTable table)
{

    char fileLoc[MAX_PATH_SIZE] = {0};
    const char* path[]          = {ASM_DIR, "config.asm"};
    safe_multi_strcat(fileLoc, path, 2, MAX_PATH_SIZE);
    FILE* file     = fopen(fileLoc, "w");
    int room_count = countRooms(table);
    int stack_size = room_count + 1; // Leave room for terminal connection 0

    fprintf(file, "section .data\n\n");
    fprintf(file, "    visited_count    dq 0\n\n");

    fprintf(file, "section .bss\n\n");
    fprintf(file, "    stack_size       equ %d\n", stack_size);
    fprintf(file, "    stack            resq stack_size\n");
    fprintf(file, "    stack_top        resq 1\n");
    fprintf(file, "    visited_rooms    resq stack_size\n");

    fclose(file);
}

void generate_map(InstructionTable table, const char* path[])
{
    char fileLoc[MAX_PATH_SIZE] = {0};
    safe_multi_strcat(fileLoc, path, 2, MAX_PATH_SIZE);
    FILE* file = fopen(fileLoc, "w");

    // Check if the table has any entries before proceeding
    if (table->count >= 1)
    {
        int room_count = countRooms(table);
        fprintf(file, "%%define ID_LEN %d\n", ID_LEN);
        fprintf(file, "%%define ROOM_COUNT %d\n\n", room_count);

        fprintf(file, "section .data\n");

        // First collect all room names for the global declaration
        int roomCount    = countRooms(table);
        char** roomNames = getRoomNames(table);

        fprintf(file, "global entry");
        for (int i = 0; i < roomCount; i++)
        {
            fprintf(file, ", room_%s", roomNames[i]);
        }
        fprintf(file, "\n\n");

        free(roomNames);
    }

    int hasEntry = 0;

    // Iterate through the instruction table and process room entries
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].InstrCode != IR_DECL_ROOM)
            continue;

        // Write entry information for the first room encountered
        if (!hasEntry)
        {
            fprintf(file, "align 8\n");
            fprintf(file, "entry: dq room_%s\n", table->entries[i].args[0]);
            hasEntry = 1;
        }

        // Allocate and initialize Room structure
        Room* currentRoom      = calloc(1, sizeof(Room));
        currentRoom->id        = table->entries[i].args[0];
        currentRoom->connCount = 0;

        // Allocate memory for room's connections
        currentRoom->connections = calloc(MAX_CONNECTIONS, sizeof(char*));

        if (!currentRoom->connections)
        {
            perror("Memory allocation failed for connections\n");
            free(currentRoom);
            fclose(file);
            return;
        }

        // Get room connections and generate room assembly
        getConnections(table, currentRoom, i + 1);
        writeRoomAssembly(currentRoom, file);
        writeConnectAssembly(currentRoom, file);

        // Free allocated memory for connections
        free(currentRoom->connections);
        free(currentRoom);
    }
    fclose(file);
}

int countRooms(InstructionTable table)
{
    int room_count = 0;

    // Count rooms in the instruction table
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].InstrCode == IR_DECL_ROOM)
            ++room_count;
    }
    return room_count;
}

char** getRoomNames(InstructionTable table)
{
    int roomCount    = 0;
    char** roomNames = calloc(table->count, sizeof(char*));

    // First pass to collect room names
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].InstrCode == IR_DECL_ROOM)
        {
            roomNames[roomCount++] = table->entries[i].args[0];
        }
    }
    return roomNames;
}

void getConnections(InstructionTable table, Room* room, int startIndex)
{
    for (int i = startIndex; i < table->count; i++)
    {
        if (table->entries[i].InstrCode != IR_DECL_CONNECT)
            continue;

        // Check if the connection belongs to the current room
        if (strcmp(table->entries[i].args[0], room->id) != 0)
            continue;

        // If max connections are reached, stop adding more
        if (room->connCount >= MAX_CONNECTIONS)
        {
            fprintf(stderr, "Too many connections for room %s\n", room->id);
            break;
        }

        room->connections[room->connCount++] = table->entries[i].args[1];
    }
}

void writeRoomAssembly(Room* room, FILE* file)
{
    if (room == NULL || room->id == NULL)
        return;

    fprintf(file, "\nalign 8\n");
    fprintf(file, "room_%s:\n", room->id);

    int roomIdLength     = strlen(room->id);              // Length of the room ID string
    int remainingPadding = MAX_INPUT_SIZE - roomIdLength; // Subtract the length of the room ID

    // Print the room ID and padding with ", 0" to match the MAX_INPUT_SIZE
    fprintf(file, "    db \"%s\"", room->id);

    // Add the remaining ", 0" to reach the required length
    fprintf(file, "\n    times %d db 0", remainingPadding);
    fprintf(file, "\n");
}

void writeConnectAssembly(Room* room, FILE* file)
{
    if (room == NULL || room->connCount < 1)
    {
        fprintf(file, "    times %d dq 0\n", MAX_CONNECTIONS);
        return;
    }

    // Write connections for the current room
    fprintf(file, "    dq ");
    for (int i = 0; i < room->connCount; i++)
    {
        fprintf(file, "room_%s", room->connections[i]);
        if (i < room->connCount - 1)
        {
            fprintf(file, ", ");
        }
    }

    fprintf(file, "\n    times %d dq 0", MAX_CONNECTIONS - room->connCount);
    fprintf(file, "\n");
}

void writeCInterface(char** rooms, int roomCount, int roomNameSize, int maxConnections)
{
    char fileLoc[MAX_PATH_SIZE] = {0};
    const char* path[]          = {GENERATED_DIR, "/interfaces/mf_interface.h"};
    safe_multi_strcat(fileLoc, path, 2, MAX_PATH_SIZE);
    FILE* file = fopen(fileLoc, "w");

    fprintf(file, "#pragma once\n\n");

    fprintf(file, "#define ROOM_COUNT %d\n", roomCount);
    fprintf(file, "#define MAX_CONNECTION %d\n", MAX_CONNECTIONS);
    fprintf(file, "#define ID_LEN %d\n\n", ID_LEN);

    // Write the Room struct definition
    fprintf(file, "typedef struct Room {\n");
    fprintf(file, "    char name[ID_LEN];           // Room name\n");
    fprintf(file, "    struct Room* connections[MAX_CONNECTION]; // Connections to other rooms\n");
    fprintf(file, "} Room;\n\n");

    // Write extern declarations for all rooms (example)
    fprintf(file, "extern Room* entry;\n");

    for (int i = 0; i < roomCount; i++)
    {
        fprintf(file, "extern Room room_%s;\n", rooms[i]);
    }
    fclose(file);
}
