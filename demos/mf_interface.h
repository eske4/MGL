#pragma once

#define ROOM_COUNT 5
#define MAX_CONNECTION 8
#define ID_LEN 32

typedef struct Room {
    char name[ID_LEN];           // Room name
    struct Room* connections[MAX_CONNECTION]; // Connections to other rooms
} Room;

extern Room* entry;
extern Room room_start;
extern Room room_dungeon_1;
extern Room room_dungeon_2;
extern Room room_end;
extern Room room_trap;
