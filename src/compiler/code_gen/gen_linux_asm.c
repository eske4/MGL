#include "code_gen.h"
#include "definitions.h"
#include "instr_table.h"
#include "file_config.c"
#include "string_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_map(InstructionTable table);
void generate_config(InstructionTable table);
void getConnections(InstructionTable table, Room* room, int startIndex);
void writeConnectAssembly(Room* room, FILE* file);
void writeRoomAssembly(Room* room, FILE* file);

void generate_assembly(InstructionTable table)
{
    printf("\n");
    printf("Generating map...\n");
    generate_map(table);
    printf("SUCCESSFULLY: generated map\n\n");

    printf("Generating configs...\n");
    generate_config(table);
    printf("SUCCESSFULLY: generated configs\n\n");
}

void generate_config(InstructionTable table)
{

    char fileLoc[MAX_PATH_SIZE] = {0};
    const char* path[]          = {GENERATED_ASM_DIR, "config.asm"};
    safe_multi_strcat(fileLoc, path, 2, MAX_PATH_SIZE);
    FILE* file     = fopen(fileLoc, "w");
    int room_count = 0;

    // Count rooms in the instruction table
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].InstrCode == IR_DECL_ROOM)
            ++room_count;
    }

    int stack_size = room_count + 1; // Leave room for terminal connection 0

    fprintf(file, "%%define ID_LEN %d\n", MAX_INPUT_SIZE);
    fprintf(file, "%%define ROOM_COUNT %d\n\n", room_count);

    fprintf(file, "section .data\n\n");
    fprintf(file, "    visited_count    dq 0\n\n");

    fprintf(file, "section .bss\n\n");
    fprintf(file, "    stack_size       equ %d\n", stack_size);
    fprintf(file, "    stack            resq stack_size\n");
    fprintf(file, "    stack_top        resq 1\n");
    fprintf(file, "    visited_rooms    resq stack_size\n");

    fclose(file);
}

void generate_map(InstructionTable table)
{
    char fileLoc[MAX_PATH_SIZE] = {0};
    const char* path[]          = {GENERATED_ASM_DIR, "map.asm"};
    safe_multi_strcat(fileLoc, path, 2, MAX_PATH_SIZE);
    FILE* file = fopen(fileLoc, "w");

    int hasEntry = 0;

    // Check if the table has any entries before proceeding
    if (table->count >= 1)
    {
        fprintf(file, "section .data\n\n");
    }

    // Iterate through the instruction table and process room entries
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].InstrCode != IR_DECL_ROOM)
            continue;

        // Write entry information for the first room encountered
        if (!hasEntry)
        {
            fprintf(file, "entry: dq room_%s\n\n", table->entries[i].args[0]);
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

    fprintf(file, "room_%s:\n", room->id);

    int roomIdLength     = strlen(room->id);              // Length of the room ID string
    int remainingPadding = MAX_INPUT_SIZE - roomIdLength; // Subtract the length of the room ID

    // Print the room ID and padding with ", 0" to match the MAX_INPUT_SIZE
    fprintf(file, "    db \"%s\"", room->id);

    // Add the remaining ", 0" to reach the required length
    for (int i = 0; i < remainingPadding; i++)
    {
        fprintf(file, ", 0");
    }

    fprintf(file, "\n");
}

void writeConnectAssembly(Room* room, FILE* file)
{
    if (room == NULL || room->connCount < 1)
    {
        fprintf(file, "    dq 0\n\n");
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
    fprintf(file, ", 0\n\n");
}
