.section .rodata
intout: .string "%ld\n"
.globl main
.section .text
main:
    pushq %rbp
    movq %rsp, %rbp
    movq $8, %rdi
    call _f
    movq %rax, %rsi
    movq $intout, %rdi
    call printf
    movq $0, %rdi
_f:
    pushq %rbp
    movq %rsp, %rbp
    movq $1, %rax
    cqo
LOOP:
    cmpq $0, %rdi
    jz END
    imulq %rdi
    subq $1, %rdi
    jmp LOOP
END:
    leave
    ret
