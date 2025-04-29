section .data
    newline db 10, 0
    space db " ", 0

section .bss
    input_buffer resb 256   ; Reserve 100 bytes for input
    input_len equ $-input_buffer

section .text
print:
    push rdi                ; store register in stack
    push rsi                
    push rdx                
    
    mov rsi, rdi            ; Copy string pointer to RSI
    call slen               ; Length returned in RAX
    
    ; Prepare syscall
    mov rdx, rax            ; length
    mov rdi, 1              ; stdout
    mov rax, 1              ; sys_write
    syscall                 ; Do the write with original RDI/RDX restored
    
    ; Restore in reverse order
    pop rdx                 ; RDX = [rsp+16]
    pop rsi                 ; RSI = [rsp+8]
    pop rdi                 ; RDI = [rsp]
    
    ret

printl:
    push    rdi             ; Save original argument (string ptr)
    call    print           ; Print the string

    ; Now print a newline
    push    rdi             ; Save original rdi again (print may have changed it)
    mov     rdi, newline    ; rdi = address of "\n"
    call    print
    pop     rdi             ; Restore original rdi

    pop     rdi             ; Restore original value
    ret

print_line:
    push    rdi

    mov     rdi, newline
    call    print

    pop     rdi
    ret

print_space:
    push    rdi

    mov     rdi, space
    call    print

    pop     rdi
    ret



print_num:
    push    rdi
    push    rsi

    call    int2str
    mov     rdi, rax
    call    print

    pop     rsi
    pop     rdi
    ret

slen:
    push    rbx             ; Save callee-saved register
    mov     rbx, rdi        ; rbx = start of string

.nextchar:
    cmp     byte [rdi], 0
    je      .done
    inc     rdi
    jmp     .nextchar

.done:
    sub     rdi, rbx
    mov     rax, rdi        ; Return result in rax (convention)
    pop     rbx
    ret

scan:
    push    rdx
    push    rsi
    push    rdi

    ; Read input (sys_read)
    xor     rax, rax          ; sys_read = 0
    xor     rdi, rdi          ; stdin = 0
    mov     rsi, input_buffer
    mov     rdx, input_len
    syscall

    ; Null-terminate and remove newline
    mov     byte [rsi + rax - 1], 0  ; Overwrite newline with null terminator

    mov     rax, rsi          ; Return buffer address
    pop     rdi
    pop     rsi
    pop     rdx
    ret

exit_program: ; Exits the program
    mov     rax, 60         ; sys_exit (correct number is 60)
    mov     rdi, 0          ; exit status
    syscall


