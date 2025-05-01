section .data

    id_len           dq 30
    room_count       dq 4
    visited_count    dq 0

section .bss

    stack_size       equ 5
    stack            resq stack_size
    stack_top        resq 1
    visited_rooms    resq stack_size
