.section .data
.global original_brk
        original_brk: .quad 0
.global current_brk 
        current_brk: .quad 0
        
.section .text
#.global _start

.global memory_alloc
memory_alloc:

        pushq %rbp
        movq %rsp, %rbp

        # Copiar o valor do parâmetro (bytes alocados) em r10
        movq %rdi, %r10

        movq current_brk, %rcx

        # %r13 será usado como valor temporário
        movq original_brk, %r13

        # %r14 = biggest_empty_block
        movq %r13, %r14

_inicio_while:
        cmp %rcx, %r13
        jge _fim_while

        # Testa se o bloco esta sendo usado
        cmpq $1, (%r13)
        je _fora_if

        # Testa se o bloco atual é maior que o maior bloco
        # (maior bloco em %r14, e tamanho do bloco atual em %r15)
        movq 8(%r13), %r15
        cmpq 8(%r14), %r15
        jle _fora_if

        # Atualiza o endereco do maior bloco
        movq %r13, %r14

_fora_if:
        movq 8(%r13), %r12
        addq %r12, %r13
        addq $16, %r13
        jmp _inicio_while
       
_fim_while:
        cmpq %r14, current_brk
        je _fora_if_3

        cmpq $1, (%r14)
        je _fora_if_3

        addq $8, %r14
        # Testa se há memoria disponível no bloco
        cmpq %rdi, (%r14)
        jl _fora_if_3

        # Marca o bloco como usado
        movq $1, -8(%r14)

        # Testa se é possível criar um segundo bloco com o que sobrou de memória
        movq (%r14), %r8
        subq %rdi, %r8
        cmp $17, %r8
        jl _fora_if_2

        # Coloca o tamanho antigo do bloco em %r9 (r9 = oldSize)
        movq (%r14), %r9
        
        # biggest_empty_block + 8 = bytes
        movq %rdi, (%r14)

        # %r11 tem o endereço onde o novo registro de memória está sendo escrito
        movq %r14, %r11
        addq %rdi, %r11
        addq $8, %r11
 
        # Marca o novo registro como não usado
        movq $0, (%r11)
        
        # Calcula o valor do novo bloco em %r9
        subq %rdi, %r9
        subq $16, %r9
        addq $8, %r11

        # Escreve o valor do novo bloco no seu registro
        movq %r9, (%r11)


_fora_if_2:
        addq $8, %r14
        movq %r14, %rax
        popq %rbp
        ret 

_fora_if_3:

        # Adiciona os 16 bytes do registro para o valor de bytes sendo
        # alocados (parâmetro passado em %rdi)
        addq current_brk, %rdi
        addq $16, %rdi

        # Syscall brk
        movq $12, %rax
        syscall

        # Ad#iciona o tamanho do registro em current brk
        addq $16, current_brk

        # Salva o inicio do bloco (após o registro) para retornar depois
        movq current_brk, %rbx

        # Coloca o novo valor de brk em current_brk retornado em %rax depois da syscall
        movq %rax, current_brk

        # Coloca o valor do início do bloco para ser retornado em %rax
        movq %rbx, %rax

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
        
        cmp %rdi, current_brk
        jl _fora_if_4

        # Marca o bloco de memória como não usado.
        # O endereço do bloco passado como parâmetro está em %rdi
        # e a quadword que indica o uso do bloco está 16 bytes atrás
        movq $0, -16(%rdi)


_fora_if_4:

        # Retornar 0 em %rax
        movq $0, %rax

        popq %rbp
        ret

