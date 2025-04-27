section .data
    newline db 0Ah, 0


section .text
print:
    push    rdi             ; Save original string pointer

    call    slen            ; returns length in rax

    ; Set up syscall arguments
    mov     rdx, rax        ; rdx = length
    pop     rsi             ; restore string pointer into rsi
    mov     rdi, 1          ; rdi = file descriptor (stdout)
    mov     rax, 1          ; syscall number for sys_write
    syscall

    ret

printl:
    push    rdi             ; Save original argument (string ptr)
    call    print           ; Print the string

    ; Now print a newline
    mov     rdi, newline    ; rdi = address of "\n"
    call    print

    pop     rdi             ; Restore original value
    ret

slen:
    push    rbx             ; Save callee-saved register
    mov     rbx, rdi        ; rbx = start of string
    jmp     .nextchar

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


exit_program: ; Exits the program
    mov     rax, 60         ; sys_exit (correct number is 60)
    mov     rdi, 0          ; exit status
    syscall
