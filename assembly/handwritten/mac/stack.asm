bits 64
default rel

section .text
global init_stack, stack_push, stack_pop

init_stack:
    mov rcx, stack_size
    lea rdi, [rel stack]
    xor rax, rax
.clear:
    mov [rdi], rax
    add rdi, 8
    loop .clear
    lea rax, [rel stack]
    mov [rel stack_top], rax
    ret

stack_push:
    push rsi
    mov rsi, rdi
    mov rdi, [rel stack_top]
    lea rax, [rel stack + stack_size*8]
    cmp rdi, rax
    jae .overflow
    mov [rdi], rsi
    add rdi, 8
    mov [rel stack_top], rdi
    pop rsi
    ret
.overflow:
    call exit_program

stack_pop:
    mov rdi, [rel stack_top]
    lea rax, [rel stack]
    cmp rdi, rax
    jbe .empty
    sub rdi, 8
    mov rax, [rdi]
    mov qword [rdi], 0
    mov [rel stack_top], rdi
    ret
.empty:
    xor rax, rax
    ret
