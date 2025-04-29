section .data
    select_msg:     db "╔════════════════════════════════════════╗", 10
                    db "║        SELECT AVAILABLE ROOMS          ║", 10
                    db "╠════════════════════════════════════════╣", 10
                    db "║ Left: Room Name      Right: Index      ║", 10
                    db "╚════════════════════════════════════════╝", 0

    error:          db "Could not be moved", 0
    no_rooms:       db "No more rooms to explore from here.", 0
    bad_input:      db "Invalid input - please try again", 0
    input_prompt:   db "Enter room index or 'q' to quit: ", 0
    quit_msg:       db "Traversal session ended", 0

section .text
manual_traversal:
    ; Start at entry
    mov     rsi, [entry]         ; Starting room address

.traversal_loop:
    ; First check if current room has any connections
    mov     rdi, [rsi + 30]     ; Check first connection
    test    rdi, rdi
    jz      .no_connections      ; If no connections, exit

    call    print_room_options   ; Print available connections
    call    select_room          ; Get user selection
    
    mov     rsi, rax            ; Move to selected room
    jmp     .traversal_loop     ; Continue traversal

.no_connections:
    mov     rdi, no_rooms
    call    printl
    ret

print_room_options:
    push    rsi
    push    rdx
    
    mov     rdi, select_msg
    call    printl
    
    mov     rdx, 0               ; Connection index
    
.print_loop:
    mov     rdi, [rsi + 30 + rdx * 8]  ; Get connection
    test    rdi, rdi
    jz      .done_printing
    
    call    print               ; Print room name
    mov     rdi, rdx
    call    print_space
    call    print_num
    call    print_line
    
    inc     rdx
    jmp     .print_loop

.done_printing:
    pop     rdx
    pop     rsi
    ret

select_room:
    push    rsi
    push    rdi
    
.input_loop:
    ; Prints "Type room index here: "
    mov     rdi, input_prompt
    call    print

    ; Requests input from user
    call    scan              ; Get input
    mov     rdi, rax
    call    printl
    
    ; Check for 'Q' or 'q' to quit
    cmp     byte [rdi], 'Q'
    je      .quit
    cmp     byte [rdi], 'q'
    je      .quit
    
    ; Convert string to int
    call    str2int           ; Convert to int (rax)
    cmp     rax, 0            ; Check if conversion failed (str2int should return -1 or similar for non-numbers)
    jl      .invalid
    
    mov     rdi, [rsi + 30 + rax * 8]
    test    rdi, rdi
    jz      .invalid
    
    ; Valid selection
    mov     rax, rdi
    pop     rdi
    pop     rsi
    ret

.invalid:
    push    rdi
    mov     rdi, bad_input
    call    printl
    pop     rdi
    jmp     .input_loop

.quit:
    mov     rdi, quit_msg
    call    printl
    xor     rax, rax          ; Return NULL to indicate quit
    pop     rdi
    pop     rsi
    ret


