section .data
    newline db 10, 0
    space db " ", 0

section .bss
    input_buffer resb 256               ; Reserve 100 bytes for input
    input_len equ $-input_buffer

section .text
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

; Prints an string followed by newline
printl:
    call    print                       ; Print the string
    push    rdi                         ; Save original rdi again (print may have changed it)

    ; Prints a newline
    mov     rdi, newline                ; rdi = address of "\n"
    call    print

    pop     rdi                         ; Restore original value
    ret

; Prints a newline
print_line:
    push    rdi

    mov     rdi, newline
    call    print

    pop     rdi
    ret

; Prints a space
print_space:
    push    rdi

    mov     rdi, space
    call    print

    pop     rdi
    ret



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

; Reads user input (returns buffer address in rax)
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

; Exits program with status 0 (same as return 0;)
exit_program: ; Exits the program
    mov     rax, 60         ; sys_exit (correct number is 60)
    mov     rdi, 0          ; exit status
    syscall


