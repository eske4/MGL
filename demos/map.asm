section .data
global entry, room_start, room_dungeon_1, room_dungeon_2, room_end, room_trap

align 8
entry: dq room_start

align 8
room_start:
    db "start"
    times 27 db 0
    dq room_dungeon_1, room_dungeon_2
    times 6 dq 0

align 8
room_dungeon_1:
    db "dungeon_1"
    times 23 db 0
    dq room_trap
    times 7 dq 0

align 8
room_dungeon_2:
    db "dungeon_2"
    times 23 db 0
    dq room_end
    times 7 dq 0

align 8
room_end:
    db "end"
    times 29 db 0
    times 8 dq 0

align 8
room_trap:
    db "trap"
    times 28 db 0
    dq room_start
    times 7 dq 0
