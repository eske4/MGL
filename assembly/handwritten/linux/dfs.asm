; ====================== DATA SECTION ======================
section .data
    ; Messages
    warning_msg:    dq "WARNING: Not all rooms could be traversed", 0
    success_msg:    dq "SUCCESS: All rooms are reachable", 0
    traverse_msg:   dq "Traversed room: ", 0

; ====================== CODE SECTION ======================
section .text

; ====================== DFS TRAVERSAL ======================
; Performs depth-first search traversal of rooms
dfs_traversal:
    ; Push starting room onto stack
    mov     rdi, [entry]
    call    stack_push

.dfs_loop:
    ; Check if stack is empty
    mov     rdi, [stack_top]
    cmp     rdi, stack
    je      .dfs_done                ; If stack is empty, we're done

    ; Get current room
    call    stack_pop
    mov     rdi, rax                ; rdi now contains current room address

    ; Check if already visited
    call    check_visited
    cmp     rax, 1
    je      .dfs_loop                ; Skip if already visited

    ; Mark as visited
    call    mark_room

    push    rdi
    mov     rdi, traverse_msg
    ; Process current room (print it)
    call    print

    pop     rdi
    call    printl

    ; Push all connected rooms onto stack
    call    get_connections

    jmp     .dfs_loop

.dfs_done:
    ret

; ====================== ROOM MARKING ======================
; Marks a room as visited
; Parameters: RDI - room address
mark_room:
    push    rdx
    push    rax

    mov     rax, visited_rooms
    mov     rdx, [visited_count]
    mov     [rax + rdx * 8], rdi    ; Store room in visited list
    inc     qword [visited_count]   ; Increment visited count

    pop     rax
    pop     rdx
    ret

; ====================== TRAVERSAL CHECK ======================
; Checks if all rooms were visited and prints appropriate message
all_room_traversed:
    mov     rax, [visited_count]
    mov     rbx, [room_count]
    cmp     rax, rbx
    je      .all_visited

    ; Not all rooms visited
    mov     rdi, warning_msg
    call    printl
    ret

.all_visited:
    mov     rdi, success_msg
    call    printl
    call    print_line
    ret

; ====================== CONNECTION HANDLING ======================
; Gets all connections for the current room and pushes unvisited ones to stack
; Parameters: RDI - current room address
get_connections:
    push    rsi
    push    rbx
    push    rcx
    push    rdi                  ; Preserve current room pointer

    mov     rsi, [id_len]       ; Get id length
    lea     rbx, [rdi + rsi]      ; Point to first connection

.process_connection:
    mov     rcx, [rbx]            ; Get connection pointer
    test    rcx, rcx
    jz      .connections_done      ; End if null

    ; Check if visited - pass pointer in RSI
    push    rbx                  ; Save connection pointer
    mov     rsi, rcx              ; Set up for check_visited
    call    check_visited
    pop     rbx                   ; Restore connection pointer
    cmp     rax, 1
    je      .next_connection       ; Skip if visited

    ; Push unvisited connection
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

; ====================== VISITED CHECK ======================
; Checks if a room has been visited
; Parameters: RSI - room address to check
; Returns: RAX - 1 if visited, 0 otherwise
check_visited:
    push    rcx
    push    rdx

    xor     rax, rax              ; Default to not visited
    mov     rcx, [visited_count]
    test    rcx, rcx
    jz      .not_visited

    mov     rdx, visited_rooms

.check_loop:
    cmp     [rdx], rsi            ; Compare with room in RSI
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
