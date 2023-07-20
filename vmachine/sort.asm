section .data
    array  dq  14, 27, 20, 07, 23
    len    equ ($ - array) / 8
section .bss
section .text
    global main
main:
    push    rbp
    mov     rbp,  rsp
    
    mov     rdi,  array
    mov     rsi,  len
    call    sort

    leave
    ret

sort:
    push    rbp
    mov     rbp, rsp

.loop1_init:

    mov     rcx, 1
    jmp     .loop1_cond

.loop1: 
.loop2_init:
    
    mov     rdx, rcx
    jmp     .loop2_cond

.loop2:

    lea     r10, [rdi + 8 * rdx] ; r10 = A + j
    lea     r11, [r10 - 8] ; r11 = A + j - 1

    mov     r12, [r10]
    mov     r13, [r11]

.if_cond:

    cmp     r12, r13 ; A[j] cmp A[j-1]
    jl      .if_branch

.else_branch:
    
    jmp     .loop2_done
;   jmp     .end_if

.if_branch:

    mov     [r10], r13
    mov     [r11], r12 ; swap(A[j], A[j-1])

.end_if:

.loop2_inc:
    
    dec     rdx

.loop2_cond:

    cmp     rdx, 0
    jg      .loop2

.loop2_done:

.loop1_inc:

    inc     rcx

.loop1_cond:

    cmp     rcx, rsi
    jl      .loop1

.loop1_done:

.done:

    leave
    ret
    

