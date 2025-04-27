%include "generated/map.asm"
%include "generated/config.asm"

%ifdef LINUX
    %include "handwritten/linux/core.asm"
    %include "handwritten/linux/stack.asm"
    %include "handwritten/linux/dfs.asm"
%endif

%ifdef WINDOWS
    %include "handwritten/win/core.asm"
    %include "handwritten/win/stack.asm"
    %include "handwritten/win/dfs.asm"
%endif

section .text
global _start
_start:
    call    init_stack              ; Initialize our stack
    call    dfs_traversal           ; Start DFS from the starting room
    call    all_room_traversed      ; Check if all rooms were visited
    call    exit_program            ; Exit
