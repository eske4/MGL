; ====================== DATA SECTION ======================
section .data
    ; Status messages
    warning_msg:    dq "WARNING: The dungeon layout contains potential dead ends. Be mindful of your choices.", 0
    success_msg:    dq "SUCCESS: The dungeon map has been designed without any dead ends. All paths lead somewhere.", 0
    traverse_msg:   dq "Traversed room: ", 0

; ====================== CODE SECTION ======================
section .text

; ====================== DFS TRAVERSAL =====================
; Input: None
; Output: None
; Modifies: Stack, visited rooms list
; 
; Performs DFS starting from entry room until all reachable rooms are visited
; Uses stack to track rooms to visit next
; ===========================================================

dfs_traversal:
    mov     rdi, [entry]
    call    stack_push

.dfs_loop:
    ; Check for completion
    mov     rdi, [stack_top]
    cmp     rdi, stack
    je      .dfs_done

    ; Process next room
    call    stack_pop
    mov     rdi, rax
    
    call    check_visited
    cmp     rax, 1
    je      .dfs_loop

    ; Mark and print room
    call    mark_room
    push    rdi
    mov     rdi, traverse_msg
    call    print
    pop     rdi
    call    printl

    ; Queue connected rooms
    call    get_connections
    jmp     .dfs_loop

.dfs_done:
    ret

; ====================== ROOM MARKING ======================
; Input: RDI - Room address to mark
; Output: None
; Modifies: visited_rooms, visited_count
; ==========================================================

mark_room:
    push    rdx
    push    rax

    mov     rax, visited_rooms
    mov     rdx, [visited_count]
    mov     [rax + rdx * 8], rdi
    inc     qword [visited_count]

    pop     rax
    pop     rdx
    ret

; ====================== TRAVERSAL CHECK ==================
; Input: None
; Output: None (prints result message)
; Checks if all rooms were visited
; ==========================================================

all_room_traversed:
    mov     rax, [visited_count]
    cmp     rax, ROOM_COUNT
    je      .all_visited

    mov     rdi, warning_msg
    call    printl
    ret

.all_visited:
    mov     rdi, success_msg
    call    printl
    call    print_line
    ret

; ====================== CONNECTION HANDLING ==============
; Input: RDI - Current room address
; Output: None
; Processes all connections for current room
; ==========================================================

get_connections:
    push    rsi
    push    rbx
    push    rcx
    push    rdi

    mov     rsi, ID_LEN
    lea     rbx, [rdi + rsi]    ; Point to connections list

.process_connection:
    mov     rcx, [rbx]
    test    rcx, rcx
    jz      .connections_done

    push    rbx
    mov     rsi, rcx
    call    check_visited
    pop     rbx
    cmp     rax, 1
    je      .next_connection

    mov     rdi, rcx
    call    stack_push

.next_connection:
    add     rbx, 8
    jmp     .process_connection

.connections_done:
    pop     rdi
    pop     rcx
    pop     rbx
    pop     rsi
    ret

; ====================== VISITED CHECK ====================
; Input: RSI - Room address to check
; Output: RAX - 1 if visited, 0 otherwise
; ==========================================================

check_visited:
    push    rcx
    push    rdx

    xor     rax, rax
    mov     rcx, [visited_count]
    test    rcx, rcx
    jz      .not_visited

    mov     rdx, visited_rooms

.check_loop:
    cmp     [rdx], rsi
    je      .is_visited
    add     rdx, 8
    dec     rcx
    jnz     .check_loop

.not_visited:
    pop     rdx
    pop     rcx
    ret

.is_visited:
    mov     rax, 1
    pop     rdx
    pop     rcx
    ret
