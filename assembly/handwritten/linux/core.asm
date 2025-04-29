section .data
    newline db 10, 0
    space db " ", 0

section .bss
    input_buffer resb 256               ; Reserve 100 bytes for input
    input_len equ $-input_buffer

section .text

; ===================== PRINT STRING ===================================================
; Takes a string input followed by a 0 terminator and prints it
; Arguments:
;   rdi - input string
; Returns:
;   NONE
; =======================================================================================

print:
    ; Store registers value before function 
    push rdi                
    push rsi                
    push rdx                
    
    mov rsi, rdi                        ; Copy string pointer to RSI
    call slen                           ; Length returned in RAX
    
    ; Prepare syscall
    mov rdx, rax                        ; length
    mov rdi, 1                          ; stdout
    mov rax, 1                          ; sys_write
    syscall                             ; Call linux kernel terminal print command
    
    ; Restore registers values before functions
    pop rdx                 
    pop rsi                 
    pop rdi
    
    ret

; ===================== PRINT STRING WITH NEWLINE ======================================
; Same as print but with newline
; Arguments:
;   rdi - input string
; Returns:
;   NONE
; =======================================================================================

printl:
    call    print                       ; Print the string
    push    rdi                         ; Save original rdi again (print may have changed it)

    ; Prints a newline
    mov     rdi, newline                ; rdi = address of "\n"
    call    print

    pop     rdi                         ; Restore original value
    ret

; ===================== PRINTS A NEWLINE ================================================
; Prints a newline
; Arguments:
;   NONE
; Returns:
;   NONE
; =======================================================================================

print_line:
    push    rdi

    mov     rdi, newline
    call    print

    pop     rdi
    ret

; ===================== PRINTS A NEWLINE ================================================
; Prints a space
; Arguments:
;   NONE
; Returns:
;   NONE
; =======================================================================================


print_space:
    push    rdi

    mov     rdi, space
    call    print

    pop     rdi
    ret



; ===================== PRINTS A NEWLINE ================================================
; Converts a number into string and prints it
; Arguments:
; rdi - number input
; Returns:
;   NONE
; =======================================================================================

print_num:
    ; Save registers value before function
    push    rdi
    push    rsi

    call    int2str                     ; Convert to string
    mov     rdi, rax                    ; Get string value
    call    print                       ; Print the number string

    ; Restore registers values before functions
    pop     rsi
    pop     rdi
    ret

; ===================== FETCH STRING LENGTH ============================================
; This function walks through the string byte-by-byte until it finds the null terminator,
; then subtracts the original pointer from the final pointer to get the length.
;
; Arguments:
;   rdi - string input
; Returns:
;   rax - length of an string
; =======================================================================================

slen:
    push    rbx
    mov     rbx, rdi                    ; remember string start address

.nextchar:
    cmp     byte [rdi], 0               ; Check for null terminator
    je      .done
    inc     rdi                         ; Move to next char
    jmp     .nextchar

.done:
    sub     rdi, rbx                    ; Calculate length
    mov     rax, rdi                    ; Return result in rax (convention)
    pop     rbx
    ret

; =============================== READ USER INPUT ========================================
; Reads a line of input from the user via Linux syscalls.
;
; Arguments:
;   None (fetches from the Linux syscall).
;
; Returns:
;   rax - Address of the buffer containing the user input (null-terminated).
; =======================================================================================

scan:
    push    rdx
    push    rsi
    push    rdi
    
    ; Setup linux kernel calls
    xor     rax, rax                    ; sys_read
    xor     rdi, rdi                    ; stdin
    mov     rsi, input_buffer           ; Buffer address
    mov     rdx, input_len              ; Buffer length
    syscall
    
    mov     byte [rsi + rax - 1], 0     ; Replace newline with null terminator
    mov     rax, rsi                    ; Return buffer address
    
    pop     rdi
    pop     rsi
    pop     rdx
    ret

; =============================== EXIT ========================================
; Exit the program with status 1 same as return 1; through Linux Syscall
;
; Arguments:
;   NONE - could add status code input through rdi
; Returns:
;   NONE
; =======================================================================================

exit_program: ; Exits the program
    mov     rax, 60         ; sys_exit (correct number is 60)
    mov     rdi, 0          ; exit status
    syscall


