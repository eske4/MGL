#include <stdio.h>
#include "mf_interface.h"
#define ID_LEN         32
#define MAX_CONNECTION 8
#define ROOM_COUNT     5

void print_room(Room* room)
{
    if (!room)
    {
        printf("Invalid room!\n");
        return;
    }

    printf("Room: %s\n", room->name);
    printf("Connections: ");

    for (int i = 0; i < ROOM_COUNT; i++)
    {
        if (room->connections[i])
        {
            printf("%s ", room->connections[i]->name);
        }
    }
    printf("\n\n");
}

int main()
{
    // Print all rooms
    printf("=== Room Structure ===\n\n");

    printf("=== Entry Room ===\n");
    print_room(entry);

    printf("===    Rooms   ===\n");
    print_room(&room_start);
    print_room(&room_dungeon_1);
    print_room(&room_dungeon_2);
    print_room(&room_trap);
    print_room(&room_end);

    // Safe traversal demo
    printf("=== Safe Traversal ===\n");
    Room* current = &room_start;
    int max_steps = ROOM_COUNT + 1;

    while (current && max_steps--)
    {
        print_room(current);
        current = current->connections[0]; // Move to first exit
    }

    return 0;
}
