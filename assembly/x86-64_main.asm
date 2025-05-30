%include "generated/map.asm"
%include "generated/config.asm"

%ifdef LINUX
    %include "handwritten/linux/core.asm"
    %include "handwritten/linux/stack.asm"
    %include "handwritten/linux/dfs.asm"
    %include "handwritten/linux/graph_walk.asm"
    %include "handwritten/linux/convert.asm"
%endif

%ifdef WIN
    %include "handwritten/win/core.asm"
    %include "handwritten/win/stack.asm"
    %include "handwritten/win/dfs.asm"
    %include "handwritten/win/graph_walk.asm"
    %include "handwritten/win/convert.asm"
%endif

%ifdef MAC
    %include "handwritten/mac/core.asm"
    %include "handwritten/mac/stack.asm"
    %include "handwritten/mac/dfs.asm"
    %include "handwritten/mac/graph_walk.asm"
    %include "handwritten/mac/convert.asm"
%endif

section .text
global _start
_start:
    call    init_stack              ; Initialize our stack
    call    dfs_traversal           ; Start DFS from the starting room
    call    all_room_traversed      ; Check if all rooms were visited
    call    graph_walk
    call    exit_program            ; Exit
