section .data
    ; Message shown when listing available rooms
    select_msg:     db "╔════════════════════════════════════════╗", 10
                    db "║        SELECT AVAILABLE ROOMS          ║", 10
                    db "╠════════════════════════════════════════╣", 10
                    db "║ Left: Room Name      Right: Index      ║", 10
                    db "╚════════════════════════════════════════╝", 0

    ; Various messages for different situations
    error:          db "Could not be moved", 0
    no_rooms:       db "No more rooms to explore from here.", 0
    bad_input:      db "Invalid input - please try again", 0
    input_prompt:   db "Enter room index or 'q' to quit: ", 0
    quit_msg:       db "Traversal session ended", 0
    oob_msg:        db "Index out of range - please enter a valid room index", 0

section .text
graph_walk:
    ; Set starting room
    mov     rsi, [entry]         ; rsi = current room address

.traversal_loop:
    ; Grab first connection of current room
    mov     rdi, [rsi + 30]      
    test    rdi, rdi             ; Is it null?
    jz      .no_connections      ; If no connection, we’re done

    call    print_room_options   ; Show user the room list
    call    select_room          ; Let them pick one
    cmp     rax, -1
    je      .exit                ; User chose to quit

    mov     rsi, rax             ; Move to selected room
    jmp     .traversal_loop      ; Loop again

.no_connections:
    mov     rdi, no_rooms
    call    printl               ; Let them know they hit a dead end
.exit:
    ret

print_room_options:
    push    rsi
    push    rdx

    mov     rdi, select_msg
    call    printl               ; Print header with column labels

    mov     rdx, 0               ; Start index at 0

.print_loop:
    mov     rdi, [rsi + 30 + rdx * 8]  ; Get connection at index
    test    rdi, rdi
    jz      .done_printing       ; Done when null pointer hit

    call    print                ; Print room name
    mov     rdi, rdx
    call    print_space
    call    print_num            ; Print index next to it
    call    print_line           ; New line

    inc     rdx
    jmp     .print_loop

.done_printing:
    pop     rdx
    pop     rsi
    ret

select_room:
    push    rsi
    push    rdi
    push    rbx

    ; Count how many rooms we can jump to
    mov     rbx, 0
.count_connections:
    mov     rdi, [rsi + 30 + rbx * 8]
    test    rdi, rdi
    jz      .input_loop
    inc     rbx
    jmp     .count_connections

.input_loop:
    mov     rdi, input_prompt
    call    print                ; Ask user for input

    call    scan                 ; Get input string
    mov     rdi, rax

    ; Check if user typed 'q' or 'Q'
    cmp     byte [rdi], 'Q'
    je      .quit
    cmp     byte [rdi], 'q'
    je      .quit

    call    str2uint             ; Try converting to unsigned integer(Int that can't be negative)
    cmp     rax, 0
    jl      .invalid             ; Failed parse = invalid

    cmp     rax, rbx
    jge     .out_of_bounds       ; Too high? Invalid

    ; Success — get selected room address
    mov     rax, [rsi + 30 + rax * 8]
    pop     rbx
    pop     rdi
    pop     rsi
    ret

.out_of_bounds:
    push    rdi
    mov     rdi, oob_msg
    call    printl               ; Tell them to stay in range
    pop     rdi
    jmp     .input_loop

.invalid:
    push    rdi
    mov     rdi, bad_input
    call    printl               ; Invalid input warning
    pop     rdi
    jmp     .input_loop

.quit:
    mov     rdi, quit_msg
    call    printl               ; Friendly goodbye
    mov     rax, -1              ; Signal quit to caller
    pop     rbx
    pop     rdi
    pop     rsi
    ret
