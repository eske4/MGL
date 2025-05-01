#include <stdio.h>

typedef struct Room
{
    char name[32];
    struct Room* connections[64];
} Room;

extern Room entry;
extern Room room_A, room_B, room_E, room_D;

void print_room(Room* room)
{
    if (!room)
    {
        printf("Invalid room!\n");
        return;
    }

    printf("Room: %s\n", room->name);
    printf("Connections: ");

    for (int i = 0; i < 3; i++)
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
    printf("=== Room Structure ===\n");
    print_room(&room_A);
    print_room(&room_B);
    print_room(&room_E);
    print_room(&room_D);

    // Safe traversal demo
    printf("=== Safe Traversal ===\n");
    Room* current = &room_A;
    int max_steps = 5;

    while (current && max_steps--)
    {
        print_room(current);
        current = current->connections[0]; // Move to first exit
    }

    return 0;
}
