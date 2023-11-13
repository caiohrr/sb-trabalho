.section .data
	original_brk: .quad 0
	current_brk: .quad 0

.section .text

.global _start

setup_brk:
	pushq %rbp
	movq %rsp, %rbp

	movq $12, %rax
	syscall

	movq %rdi, original_brk
	movq %rdi, current_brk
	
	popq %rbp
	ret

dismiss_brk:
	pushq %rbp
	movq %rsp, %rbp

	movq original_brk, %rdi
	movq %rdi, current_brk

	movq $12, %rax
	syscall

	popq %rbp
	ret

rudimentaryAlloc:
	pushq %rbp
	movq %rsp, %rbp

	movq 24(%rbp), %rax
	movq current_brk, %rsi

	addq $16, %rsi
	addq %rax, %rsi

	movq %rsi, %rdi
	movq $12, %rax
	syscall

	subq %rax, %rsi
	subq $8, %rsi
	movq %rax, (%rsi)
	subq $8, %rsi
	movq $1, (%rsi)
	addq $16, %rsi

	movq %rsi, %rax

	popq %rbp
	ret	

_start:

	call setup_brk
	
	pushq $100
	call rudimentaryAlloc

	call dismiss_brk

	movq $60, %rax
	movq $0, %rdi

	syscall
