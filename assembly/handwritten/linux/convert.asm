section .data
    conv_err: db "Error converting", 0

section .bss
    int_buffer resb 101   ; 100 digits + null terminator

section .text
int2str:
    push    rbx
    push    rcx
    push    rdx
    push    rdi        ; Save original number

    mov     rax, rdi   ; Number to convert
    mov     rbx, int_buffer + 100  ; Point to the end of the buffer
    mov     byte [rbx], 0  ; Null terminator
    mov     rcx, 10        ; Base 10 divisor

    ; Handle 0 case specially
    test    rax, rax
    jnz     .convert_loop
    dec     rbx
    mov     byte [rbx], '0'
    jmp     .done

.convert_loop:
    xor     rdx, rdx
    div     rcx
    dec     rbx
    add     dl, '0'
    mov     [rbx], dl
    test    rax, rax
    jnz     .convert_loop

.done:
    mov     rax, rbx   ; Return pointer to the string
    pop     rdi
    pop     rdx
    pop     rcx
    pop     rbx
    ret

str2int:
    push    rdi
    push    rcx
    push    rdx        ; Preserve additional register we'll use

    xor     rax, rax   ; result = 0
    xor     rcx, rcx   ; digit counter

.convert_loop:
    mov     dl, byte [rdi]  ; Get next character (using DL)
    test    dl, dl          ; Check for null terminator
    jz      .success        ; End of string - success

    ; Skip leading whitespace (optional)
    cmp     rdx, ' '
    je      .skip_char
    cmp     rdx, 10         ; newline
    je      .skip_char
    cmp     rdx, 13         ; carriage return
    je      .skip_char

    ; Check if character is a digit (0-9)
    cmp     rdx, '0'
    jb      .error
    cmp     rdx, '9'
    ja      .error

    ; Convert digit and add to result
    sub     rdx, '0'        ; convert ASCII to int
    imul    rax, rax, 10    ; rax *= 10
    add     rax, rdx        ; rax += digit
    inc     rdi
    jmp     .convert_loop

.skip_char:
    inc     rdi             ; skip this character
    jmp     .convert_loop

.error:
    ; Debug output (preserve registers)
    push    rax
    push    rdi
    
    mov     rdi, conv_err
    call    printl
    
    pop     rdi
    pop     rax
    
    mov     rax, -1         ; return -1 on error
    stc                     ; set Carry Flag (CF=1)
    jmp     .done

.success:
    clc                     ; clear Carry Flag (CF=0)

.done:
    ; Optional debug output
    ; push    rax
    ; push    rdi
    ; mov     rdi, rax
    ; call    print_num
    ; pop     rdi
    ; pop     rax

    pop     rdx
    pop     rcx
    pop     rdi
    ret
