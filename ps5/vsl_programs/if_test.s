.section .rodata
.intout: .string "%ld "
.strout: .string "%s "
.errout: .string "Wrong number of arguments"
.STR0: .string "A equals 10"
.STR1: .string "B is greater than -15"
.STR2: .string "B is smaller than or equal to -15"
.section .data
.globl main
.section .text
main:
	pushq   %rbp
	movq    %rsp, %rbp
	subq	$1,%rdi
	cmpq	$0,%rdi
	jne	ABORT
	cmpq	$0,%rdi
	jz	SKIP_ARGS
	movq	%rdi,%rcx
	addq $0, %rsi
PARSE_ARGV:
	pushq %rcx
	pushq %rsi
	movq	(%rsi),%rdi
	movq	$0,%rsi
	movq	$10,%rdx
	call	strtol
	popq %rsi
	popq %rcx
	pushq %rax
	subq $8, %rsi
	loop PARSE_ARGV
SKIP_ARGS:
	call	_while_test
	jmp	END
ABORT:
	movq	$.errout, %rdi
	call puts
END:
	movq    %rax, %rdi
	call    exit
_while_test:
	pushq   %rbp
	movq    %rsp, %rbp
	subq $16, %rsp
	movq	$10, %rax
	movq	%rax, -8(%rbp)
	movq	$-15, %rax
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rsi
	movq	$.intout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
	movq	-8(%rbp), %rax
movq %rax,%rbx
	movq	$10, %rax
cmpq %rbx,%rax
jne	ENDIF0
	movq	$.STR0, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
ENDIF0:
	movq	-8(%rbp), %rax
movq %rax,%rbx
	movq	$0, %rax
cmpq %rbx,%rax
jge	ENDIF1
	movq	-16(%rbp), %rax
movq %rax,%rbx
	movq	$-15, %rax
cmpq %rbx,%rax
jge	ELSE2
	movq	$.STR1, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
	jmp	ENDIF2
ELSE2:
	movq	$.STR2, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
ENDIF2:
ENDIF1:
	movq	$0, %rax
	leave
	ret
