.section .data
        original_brk: .quad 0
        current_brk: .quad 0
        
.section .text

.global memory_alloc
memory_alloc:

        pushq %rbp
        movq %rsp, %rbp


        # Copiar o valor do parâmetro (bytes alocados) em r10
        movq %rdi, %r10

        # %r11 será usado como valor temporário
        movq current_brk, %r11

_inicio_while:
        cmp %r11, current_brk
        jge _fim_while
        
        movq -8(%r11), %r12
        addq %r12, %r11
        jmp _inicio_while
        
_fim_while:
        # Adiciona os 16 bytes do registro para o valor de bytes sendo
        # alocados (parâmetro passado em %rdi)
        addq current_brk, %rdi
        addq $16, %rdi

        # Syscall brk
        movq $12, %rax
        syscall

        # Adiciona o tamanho do registro em current brk
        addq $16, current_brk

        # Coloca o valor de current brk em %rax para retorná-lo
        movq current_brk, %rax

        # Escreve o registro de memoria
        movq $1, -16(%rax)
        movq %r10, -8(%rax)

        popq %rbp
        ret


.global setup_brk
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

.global dismiss_brk
dismiss_brk:
        
        pushq %rbp
        movq %rsp, %rbp

        # Coloca o valor de original_brk em %rdi
        # Atualiza o valor de current_brk para original_brk
	movq original_brk, %rdi
	movq %rdi, current_brk

        # Valor de original_brk (em %rdi) é chamado na syscall
        # para restaurar brk
	movq $12, %rax
	syscall

        popq %rbp
        ret

.global memory_free
memory_free:

        pushq %rbp
        movq %rsp, %rbp
        
        # Marca o bloco de memória como não usado.
        # O endereço do bloco passado como parâmetro está em %rdi
        # e a quadword que indica o uso do bloco está 16 bytes atrás
        movq $0, -16(%rdi)

        # Retornar 0 em %rax
        movq $0, %rax

        popq %rbp
        ret
