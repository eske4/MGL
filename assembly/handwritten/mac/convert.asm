bits 64
default rel

section .data
    conv_err: db "Error converting", 0

section .bss
    int_buffer resb 1001            ; 100 digits + null terminator

section .text
; ===================== INT TO STRING CONVERTER =========================================
; Converts a 64-bit unsigned integer to a null-terminated string
; Arguments:
;   rdi - unsigned 64-bit integer to convert
; Returns:
;   rax - pointer to the resulting null-terminated string in int_buffer
; ========================================================================================

int2str:
    ; Save original values for registers
    push    rbx
    push    rcx
    push    rdx
    push    rdi                     ; Save original number

    mov     rax, rdi                ; Number to convert
    lea     rbx, [rel int_buffer + 1000]  ; Point to the end of the buffer
    mov     byte [rbx], 0           ; Null terminator
    mov     rcx, 10                 ; Base 10 divisor

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
    mov     rax, rbx                ; Return pointer to the string
    pop     rdi
    pop     rdx
    pop     rcx
    pop     rbx
    ret

; ===================== STRING TO INT CONVERTER =========================================
; Converts a null-terminated string to an unsigned 64-bit integer(Only positive numbers)
; Arguments:
;   rdi - input string
; Returns:
;   rax - converted number, or -1 on error
; =======================================================================================

str2uint:
    ; Save original values for registers
    push    rdi
    push    rdx     

    mov     rax, 0                  ; Current result

.convert_loop:
    movzx   edx, byte [rdi]          ; Get next character (using DL) DL to ensure right size for int type
    test    dl, dl                  ; Check for null terminator end of string
    jz      .done                   ; End of string - success

    ; Skip leading whitespace (optional)
    cmp     rdx, ' '                ; Space
    je      .skip_char
    cmp     rdx, 10                 ; Line feed (\n)
    je      .skip_char
    cmp     rdx, 13                 ; Carriage return (\r)
    je      .skip_char

    ; Check if character is a digit (0-9)
    cmp     rdx, '0'
    jb      .error                  ; Character is below '0' in ASCII table
    cmp     rdx, '9'        
    ja      .error                  ; Character is above '9' in ASCII table

    ; Convert digit and add to result
    sub     rdx, '0'                ; Convert from ASCII to numerical value (0-9)
    imul    rax, rax, 10            ; Multiply current result by 10 (decimal shift left)
    add     rax, rdx                ; Add new digit to result
    inc     rdi                     ; Move to next character in string
    jmp     .convert_loop           ; Continue processing

.skip_char:
    inc     rdi                     ; Skip current whitespace character
    jmp     .convert_loop           ; Continue with next character

.error:
    mov     rax, -1                 ; return error code -1
    jmp     .done

.done:
    ; Restore registers back to their values before the function
    pop     rdx
    pop     rdi
    ret
