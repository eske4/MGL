section .text

init_stack:
    mov rcx, stack_size
    mov rdi, stack
    xor rax, rax

.clear_loop:
    mov [rdi], rax
    add rdi, 8
    loop .clear_loop
    mov qword [stack_top], stack
    ret

; Push value (address in rdi)
stack_push:
    push rdi
    push rsi

    mov rsi, rdi            ; Save the value we want to push
    mov rdi, [stack_top]
    
    ; Check for stack overflow
    cmp rdi, stack + stack_size * 8
    jae .overflow

    mov [rdi], rsi          ; Store the actual value
    add rdi, 8
    mov [stack_top], rdi

    pop rsi
    pop rdi
    ret
.overflow:
    call    exit_program

; Pop value into rax
stack_pop:
    mov rdi, [stack_top]
    cmp rdi, stack
    jbe .empty

    sub rdi, 8
    mov rax, [rdi]
    mov qword [rdi], 0
    mov [stack_top], rdi
    ret
.empty:
    xor rax, rax
    ret
