section .data
global _entry, _room_start, _room_dungeon_1, _room_dungeon_2, _room_end, _room_trap

align 8
_entry: dq _room_start

align 8
_room_start:
    db "start"
    times 27 db 0
    dq _room_dungeon_1, _room_dungeon_2
    times 6 dq 0

align 8
_room_dungeon_1:
    db "dungeon_1"
    times 23 db 0
    dq _room_trap
    times 7 dq 0

align 8
_room_dungeon_2:
    db "dungeon_2"
    times 23 db 0
    dq _room_end
    times 7 dq 0

align 8
_room_end:
    db "end"
    times 29 db 0
    times 8 dq 0

align 8
_room_trap:
    db "trap"
    times 28 db 0
    dq _room_start
    times 7 dq 0
