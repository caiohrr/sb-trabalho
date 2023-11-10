#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "memalloc.h"

typedef struct MemoryRegister {
        unsigned long int use, bytes;
} MemoryRegister;

void printMemReg(MemoryRegister mem_reg) {
        printf("use = %ld, bytes = %ld\n", mem_reg.use, mem_reg.bytes);
}

void setup_brk() {
        original_brk = sbrk(0);
}

void dismiss_brk() {
        brk(original_brk);
}
                    
void *memory_alloc(unsigned long int bytes) {

        MemoryRegister mem_reg;
        void *current_brk = sbrk(0);

        // Heap ainda nao foi alocada
        if (original_brk == current_brk) {
                sbrk(bytes + sizeof(MemoryRegister));
                mem_reg.use = 1;
                mem_reg.bytes = bytes;
                *(MemoryRegister *) current_brk = mem_reg;
                current_brk += sizeof(MemoryRegister);

                return current_brk;
        } else {

                void *tmp_brk = original_brk;
                MemoryRegister current_mem_reg;

                while ((unsigned long int) tmp_brk < (unsigned long int) current_brk) {
                        current_mem_reg = *(MemoryRegister *) tmp_brk;

                        // Bloco atual está em uso, se for o caso pula para o próximo
                        if (current_mem_reg.use == 1) {
                                tmp_brk += sizeof(MemoryRegister) + current_mem_reg.bytes;
                        } else {
                                // Bloco não está em uso e comporta a nova alocação
                                if (current_mem_reg.bytes > bytes) {

                                        // Escreve o novo registro de alocação em cima do anterior
                                        mem_reg.use = 1;
                                        mem_reg.bytes = bytes;
                                        *(MemoryRegister *) tmp_brk = mem_reg;
                                        void *return_brk = (tmp_brk + sizeof(MemoryRegister));

                                        // Escreve o registro que "sobrou"
                                        mem_reg.use = 0;
                                        mem_reg.bytes = current_mem_reg.bytes - (bytes + sizeof(MemoryRegister));
                                        *(MemoryRegister *) ((unsigned long int) tmp_brk + bytes + sizeof(MemoryRegister)) = mem_reg;

                                        return return_brk;
                                }

                                // Bloco não está em uso e não comporta a nova alocação
                                tmp_brk += sizeof(MemoryRegister) + current_mem_reg.bytes;
                        }
                }

                // Não foi encontrado um bloco disponível, incrementa o valor de brk;
                sbrk(bytes + sizeof(MemoryRegister));
                mem_reg.use = 1;
                mem_reg.bytes = bytes;
                *(MemoryRegister *) tmp_brk = mem_reg;
                current_brk += sizeof(MemoryRegister);

                return current_brk;
        }
}

int memory_free(void *pointer) {

        void *current_brk = sbrk(0);
        // Se o ponteiro aponta para um valor acima de brk retorna erro
        // Não sei se é necessário testar
        if ((unsigned long int) pointer > (unsigned long int) current_brk)
                return -1;

        // Escreve 0 em cima do valor que indica se o bloco está sendo usado
        *(unsigned long int *) (pointer - sizeof(MemoryRegister)) = 0;

        return 0;
}

void double_alloc_after_free(void **pointers_array){
	pointers_array[1] = memory_alloc(50);
	pointers_array[2] = memory_alloc(100);
}

void simple_alloc(void **pointers_array){
	pointers_array[0] = memory_alloc(100);
}

int main() { 

	printf("============================== ROTINAS DE TESTE ==============================\n");
	
	setup_brk();
	void *initial_brk = original_brk;
	void *alloc_pointers[3];

	simple_alloc(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[0]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	memory_free(alloc_pointers[0]);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	double_alloc_after_free(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]+100 == alloc_pointers[2]-16 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");
	printf("==> VERIFICANDO A FRAGMENTAÇÃO DE MEMÓRIA:\n");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]+50)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]+58)) == 34 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO TUDO:\n");
	memory_free(alloc_pointers[1]);
	memory_free(alloc_pointers[2]);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");


	printf("==>> DESALOCANDO A PILHA (ILEGAL):\n");
	unsigned int alloc_return = memory_free((void*) alloc_pointers);
	if (!alloc_return) printf("\tO RETORNO DA LIBERAÇÃO FOI NULL!\n");

	return 0;
}
