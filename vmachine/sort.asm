section .data
    array  dq  14, 27, 20, 07, 23
    size   equ ($ - array) / 8
section .bss
section .text
    global main
main:
    push    rbp
    mov     rbp,  rsp
    
    mov     rdi,  array
    mov     rsi,  size
    call    sort

    leave
    ret

; bubble sort. rdi = array, rsi = size
sort:
    push    rbp
    mov     rbp, rsp

; outer loop for( int i = 1; i < size; ++i) 
.loop1_init: 

    mov     rcx, 1                ;  i = 1
    jmp     .loop1_cond

.loop1: 

; inner loop for(int j = i; j > 0; ++i) 
.loop2_init:
    
    mov     rdx, rcx              ; j = i
    jmp     .loop2_cond

.loop2:

    lea     r10, [rdi + 8 * rdx]  ; r10 = A + j   
    lea     r11, [r10 - 8]        ; r11 = A + j - 1

    mov     r12, [r10]            ; r12 = *(A + j)
    mov     r13, [r11]            ; r13 = *(A + j - 1)

.if_cond:

    cmp     r12, r13              
    jl      .if_branch            ; if (A[j] < A[j-1])

.else_branch:
    
    jmp     .loop2_done
;   jmp     .end_if

.if_branch:

    mov     [r10], r13
    mov     [r11], r12            ; swap(A[j], A[j-1])

.end_if:

.loop2_inc:
    
    dec     rdx                   ; --j

.loop2_cond:

    cmp     rdx, 0
    jg      .loop2                ; j > 0

.loop2_done:

.loop1_inc:

    inc     rcx                   ; ++i

.loop1_cond:

    cmp     rcx, rsi
    jl      .loop1                ; i < size

.loop1_done:

.done:

    leave
    ret
    

