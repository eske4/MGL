%include "generated/map.asm"
%include "generated/config.asm"
%include "handwritten/core.asm"
%include "handwritten/stack.asm"
%include "handwritten/dfs.asm"

section .text
global _start

_start:
    call    init_stack              ; Initialize our stack
    call    dfs_traversal           ; Start DFS from the starting room
    call    all_room_traversed      ; Check if all rooms were visited
    call    exit_program            ; Exit gracefully
