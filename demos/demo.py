import sys
from ctypes import *

MAX_CONNECTIONS = 4
ID_LEN = 32
ROOM_COUNT = 5


class Room(Structure):
    pass


Room._fields_ = [
    ("name", c_char * ID_LEN),
    ("connections", POINTER(Room) * MAX_CONNECTIONS),
]


try:
    lib = CDLL("./map.so")
except Exception as e:
    print(f"Failed to load library: {e}", file=sys.stderr)
    sys.exit(1)


# Get rooms with validation
def get_room(name):
    try:
        room = Room.in_dll(lib, name)
        # Return a pointer to the room
        return pointer(room)
    except Exception as e:
        print(f"Failed to get room {name}: {e}", file=sys.stderr)
        return None


def get_entry(name):
    try:
        # This is a POINTER(Room) in C, so treat it as such
        return POINTER(Room).in_dll(lib, name)
    except Exception as e:
        print(f"Failed to get entry {name}: {e}", file=sys.stderr)
        return None


rooms = {
    "entry": get_entry("entry"),
    "start": get_room("room_start"),
    "dungeon1": get_room("room_dungeon_1"),
    "dungeon2": get_room("room_dungeon_2"),
    "trap": get_room("room_trap"),
    "end": get_room("room_end"),
}


def safe_deref(ptr):
    try:
        return ptr.contents if ptr else None
    except:
        return None


def print_room(room_ptr):
    room = safe_deref(room_ptr)
    if not room:
        print("Invalid room!")
        return

    print(f"Room: {room.name.decode('utf-8', errors='replace')}")
    print("Connections: ", end="")

    for i in range(MAX_CONNECTIONS):
        exit_ptr = room.connections[i]
        exit_room = safe_deref(exit_ptr)
        if exit_room:
            print(f"{exit_room.name.decode('utf-8', errors='replace')} ", end="")
    print("\n")


if __name__ == "__main__":
    print("=== Memory Validation ===")
    for name, room in rooms.items():
        print(f"Room {name} at {addressof(room) if room else 'NULL'}")

    print("\n=== Room Structure ===\n")
    for name, room in rooms.items():
        if name == "entry":
            print(f"=== Entry Room ===")
            # Directly use the address stored in entry and print the room it points to
            print_room(room)  # Now it dereferences correctly and points to room_A
            print(f"==================\n")

        else:
            print_room(room)  # Pass the room directly, since it's already a pointer

    print("=== Safe Traversal ===")
    current = rooms["entry"] if rooms["entry"] else None
    max_steps = ROOM_COUNT + 1

    while current and max_steps > 0:
        print_room(current)
        next_ptr = safe_deref(current).connections[0] if safe_deref(current) else None
        current = next_ptr if next_ptr else None
        max_steps -= 1
