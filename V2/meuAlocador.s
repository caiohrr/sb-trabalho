.section .data
	original_brk: .quad 0
	current_brk: .quad 0

.section .text

.global _start

setup_brk:
        pushq %rbp
        movq %rsp, %rbp

        # Syscall brk, retorna o valor atual da brk
        movq $0, %rdi
        movq $12, %rax
        syscall

        movq %rax, original_brk
        movq %rax, current_brk

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

	addq current_brk, %rdi
	addq $16, %rdi

	movq $12, %rax
	syscall

	movq current_brk, %rax
	addq $16, %rax

	movq $1, -16(%rax)
	movq %rdi, -8(%rax)

	popq %rbp
	ret	

_start:

	call setup_brk
	
	movq $10, %rdi
	call rudimentaryAlloc

	call dismiss_brk

	movq $60, %rax
	movq $0, %rdi

	syscall
