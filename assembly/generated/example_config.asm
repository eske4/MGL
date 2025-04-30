%define ID_LEN 30
%define ROOM_COUNT 4


section .data

    visited_count    dq 0

section .bss

    stack_size       equ 5
    stack            resq stack_size
    stack_top        resq 1
    visited_rooms    resq stack_size
