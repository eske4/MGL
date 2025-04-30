import sys
from ctypes import *


class Room(Structure):
    pass


Room._pack_ = 1
Room._fields_ = [("name", c_char * 30), ("exits", POINTER(Room) * 3)]

try:
    lib = CDLL("./map.so")
except Exception as e:
    print(f"Failed to load library: {e}", file=sys.stderr)
    sys.exit(1)


# Get rooms with validation
def get_room(name):
    try:
        room = Room.in_dll(lib, name)
        # Validate by checking first byte of name
        if not room.name[0]:
            print(f"Warning: Room {name} has empty name", file=sys.stderr)
        return room
    except Exception as e:
        print(f"Failed to get room {name}: {e}", file=sys.stderr)
        return None


rooms = {
    "A": get_room("room_A"),
    "B": get_room("room_B"),
    "E": get_room("room_E"),
    "D": get_room("room_D"),
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
    print("Exits: ", end="")

    for i in range(3):
        exit_ptr = room.exits[i]
        exit_room = safe_deref(exit_ptr)
        if exit_room:
            print(f"{exit_room.name.decode('utf-8', errors='replace')} ", end="")
    print("\n")


if __name__ == "__main__":
    print("=== Memory Validation ===")
    for name, room in rooms.items():
        print(f"Room {name} at {addressof(room) if room else 'NULL'}")

    print("\n=== Room Structure ===")
    for room in rooms.values():
        if room:
            print_room(pointer(room))

    print("=== Safe Traversal ===")
    current = pointer(rooms["A"]) if rooms["A"] else None
    max_steps = 5

    while current and max_steps > 0:
        print_room(current)
        next_ptr = safe_deref(current).exits[0] if safe_deref(current) else None
        current = next_ptr if next_ptr else None
        max_steps -= 1
