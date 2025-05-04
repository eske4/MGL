section .data

    visited_count    dq 0

section .bss

    stack_size       equ 6
    stack            resq stack_size
    stack_top        resq 1
    visited_rooms    resq stack_size
