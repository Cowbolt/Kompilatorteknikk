.section .rodata
intout: .string "%ld "
strout: .string "%s "
errout: .string "Wrong number of arguments"
.section .data
.globl main
.section .text
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $1, %rdi
	cmpq	$8,%rdi
	jne ABORT
	cmpq $0, %rdi
	jz SKIP_ARGS
	movq %rdi, %rcx
	addq $64, %rsi
PARSE_ARGV:
	pushq %rcx
	pushq %rsi
	movq (%rsi), %rdi
	movq $0, %rsi
	movq $10, %rdx
	call strtol
	popq %rsi
	popq %rcx
	pushq %rax
	subq $8, %rsi
	loop PARSE_ARGV
	popq	%rdi
	popq	%rsi
	popq	%rdx
	popq	%rcx
	popq	%r8
	popq	%r9
SKIP_ARGS:
	call	_hello
	jmp END
ABORT:
	movq $errout, %rdi
	call puts
END:
	movq %rax, %rdi
	call exit
_hello:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rdi
	pushq	%rsi
	pushq	%rdx
	pushq	%rcx
	pushq	%r8
	pushq	%r9
	pushq	%rdx
	movq	24(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	16(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-48(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-40(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-32(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-24(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-16(%rbp), %rax
	pushq	%rax
	movq	-8(%rbp), %rax
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	mulq	(%rsp)
	popq	%rdx
	popq	%rdx
	movq	%rax, %rsi
	movq	$intout, %rdi
	call	 printf
	movq	$0x0A, %rdi
	call	putchar
