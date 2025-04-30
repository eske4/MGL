bits 64
default rel

section .data
warning_msg  dq "WARNING: Not all rooms could be traversed", 0
success_msg  dq "SUCCESS: All rooms are reachable", 0
traverse_msg dq "Traversed room: ", 0

section .text
global dfs_traversal, mark_room, all_room_traversed
global get_connections, check_visited

dfs_traversal:
    mov  rdi, [rel entry]
    call stack_push
.dfs_loop:
    mov  rdi, [rel stack_top]
    lea  rax, [rel stack]
    cmp  rdi, rax
    je   .dfs_done
    call stack_pop
    mov  rdi, rax
    call check_visited
    cmp  rax, 1
    je   .dfs_loop
    call mark_room
    push rdi
    lea  rdi, [rel traverse_msg]
    call print
    pop  rdi
    call printl
    call get_connections
    jmp  .dfs_loop
.dfs_done:
    ret

mark_room:
    push rdx
    push rax
    lea  rax, [rel visited_rooms]
    mov  rdx, [rel visited_count]
    mov  [rax + rdx*8], rdi
    inc  qword [rel visited_count]
    pop  rax
    pop  rdx
    ret

all_room_traversed:
    mov  rax, [rel visited_count]
    cmp  rax, ROOM_COUNT
    je   .all_ok
    lea  rdi, [rel warning_msg]
    call printl
    ret
.all_ok:
    lea  rdi, [rel success_msg]
    call printl
    call print_line
    ret

get_connections:
    push rsi
    push rbx
    push rcx
    push rdi
    mov  rsi, ID_LEN
    lea  rbx, [rdi + rsi]
.next_conn:
    mov  rcx, [rbx]
    test rcx, rcx
    jz   .done
    push rbx
    mov  rsi, rcx
    call check_visited
    pop  rbx
    cmp  rax, 1
    je   .skip
    mov  rdi, rcx
    call stack_push
.skip:
    add  rbx, 8
    jmp  .next_conn
.done:
    pop  rdi
    pop  rcx
    pop  rbx
    pop  rsi
    ret

check_visited:
    push rcx
    push rdx
    xor  rax, rax
    mov  rcx, [rel visited_count]
    test rcx, rcx
    jz   .not_found
    lea  rdx, [rel visited_rooms]
.loop:
    cmp  [rdx], rsi
    je   .found
    add  rdx, 8
    dec  rcx
    jnz  .loop
.not_found:
    pop  rdx
    pop  rcx
    ret
.found:
    mov  rax, 1
    pop  rdx
    pop  rcx
    ret
