; ───────────── handwritten/mac/core.asm (NASM, Mach-O x86-64) ──────────────
; Exports:  print, printl, print_line, print_space, print_num,
;           scan, exit_program, slen
; Uses macOS syscall convention:  rax = 0x2000000 | BSD_nr, then `syscall`
; ───────────────────────────────────────────────────────────────────────────

bits 64
default rel                 ; make *memory* refs RIP-relative automatically

%define SYS_READ   0x2000003
%define SYS_WRITE  0x2000004
%define SYS_EXIT   0x2000001

section .data
newline  db 10,0
space    db " ",0

section .bss
input_buffer  resb 256
input_len     equ  $-input_buffer

section .text
global  print, printl, print_line, print_space, print_num
global  scan, exit_program, slen

; ───────────────────────────────── print ───────────────────────────────────
; rdi = NUL-terminated string
print:
    push rdi
    push rsi
    push rdx

    mov  rsi, rdi
    call slen
    mov  rdx, rax        ; len
    mov  rdi, 1          ; stdout
    mov  rax, SYS_WRITE
    syscall

    pop  rdx
    pop  rsi
    pop  rdi
    ret

; ─────────────────────────────── printl ────────────────────────────────────
printl:
    call print
    push rdi
    lea  rdi, [rel newline]
    call print
    pop  rdi
    ret

print_line:
    push rdi
    lea  rdi, [rel newline]
    call print
    pop  rdi
    ret

print_space:
    push rdi
    lea  rdi, [rel space]
    call print
    pop  rdi
    ret

print_num:                  ; rdi = unsigned 64-bit
    push rdi
    push rsi
    call int2str            ; from convert.asm
    mov  rdi, rax
    call print
    pop  rsi
    pop  rdi
    ret

; ─────────────────────────────── slen ──────────────────────────────────────
; rdi = str → rax = length
slen:
    push rbx
    mov  rbx, rdi
.loop:
    cmp  byte [rdi], 0
    je   .done
    inc  rdi
    jmp  .loop
.done:
    sub  rdi, rbx
    mov  rax, rdi
    pop  rbx
    ret

; ─────────────────────────────── scan ──────────────────────────────────────
; returns rax = &buffer (newline stripped)
scan:
    push rdx
    push rsi
    push rdi

    xor  rdi, rdi                 ; stdin
    lea  rsi, [rel input_buffer]
    mov  rdx, input_len
    mov  rax, SYS_READ
    syscall
    ; rax = bytes read
    lea  rsi, [rel input_buffer]
    mov  byte [rsi + rax - 1], 0  ; turn '\n' into NUL
    mov  rax, rsi

    pop  rdi
    pop  rsi
    pop  rdx
    ret

; ─────────────────────────── exit_program ──────────────────────────────────
exit_program:
    mov  rax, SYS_EXIT
    xor  rdi, rdi
    syscall
