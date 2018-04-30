.section .rodata
.intout: .string "%ld "
.strout: .string "%s "
.errout: .string "Wrong number of arguments"
.STR0: .string "Bigger"
.STR1: .string "Smaller"
.STR2: .string "Equal"
.section .data
.globl main
.section .text
main:
	pushq   %rbp
	movq    %rsp, %rbp
	subq	$1,%rdi
	cmpq	$1,%rdi
	jne	ABORT
	cmpq	$0,%rdi
	jz	SKIP_ARGS
	movq	%rdi,%rcx
	addq $8, %rsi
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
	popq	%rdi
SKIP_ARGS:
	call	_test
	jmp	END
ABORT:
	movq	$.errout, %rdi
	call puts
END:
	movq    %rax, %rdi
	call    exit
_test:
	pushq   %rbp
	movq    %rsp, %rbp
	pushq	%rdi
	pushq	$0 /* Stack padding for 16-byte alignment */
	movq	-8(%rbp), %rsi
	movq	$.intout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
	movq	-8(%rbp), %rax
movq %rax,%rbx
	movq	$10, %rax
cmpq %rbx,%rax
jge	ENDIF0
	movq	$.STR0, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
ENDIF0:
	movq	-8(%rbp), %rax
movq %rax,%rbx
	movq	$10, %rax
cmpq %rbx,%rax
jle	ENDIF1
	movq	$.STR1, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
ENDIF1:
	movq	-8(%rbp), %rax
movq %rax,%rbx
	movq	$10, %rax
cmpq %rbx,%rax
jne	ENDIF2
	movq	$.STR2, %rsi
	movq	$.strout, %rdi
	call	printf
	movq	$0x0A, %rdi
	call	putchar
ENDIF2:
	movq	$0, %rax
	leave
	ret
