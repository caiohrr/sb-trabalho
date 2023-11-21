#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include "memalloc.h"

void *original_brk;
void *brk_current;

#define lluint long long unsigned int

void *setup_brk(){
	original_brk = sbrk(0);
	brk_current = original_brk;
}

void dismiss_brk(){
	brk_current = original_brk;
	brk(original_brk);
} 

void *memory_alloc(unsigned long int bytes){
	void *tmp_brk = original_brk; 
        void *biggest_empty_block = original_brk;

	/* Laço para analisar os valores contidos na Heap. Para quando tmp_brk for maior q
	brk_current*/
	while((unsigned long long)tmp_brk < (unsigned long long)brk_current){
                //encontra o maior bloco

                // Bloco atual nao esta sendo usado e o bloco atual e o maior
                if (*(unsigned long long int *)tmp_brk == 0 && *(unsigned long long int *)(tmp_brk + 8) > *(unsigned long long int *)(biggest_empty_block + 8))
                        biggest_empty_block = tmp_brk;

		tmp_brk += *((unsigned long long int*)(tmp_brk+8))+16;
	}

        /* Analisa se o bloco esta em uso e se o tamanho do bloco é maior/igual aos bytes pedidos */
        if(biggest_empty_block != brk_current && !*((unsigned long long int *)biggest_empty_block) && (*((unsigned long long int *)(biggest_empty_block+8)) >= bytes)){

                /* Marca o bloco como em uso */
                *((unsigned long long int *) (biggest_empty_block)) = 1; 

                /* Caso o tamanho do bloco atual menos os bytes pedidos e o tamanho das 2 quadwords for maior que 1
                cria um segundo bloco */ 
                if(*((unsigned long long int *)(biggest_empty_block+8)) - bytes >= 17){
                        unsigned long long int oldSize = *((unsigned long long int*) (biggest_empty_block+8));

                        *((unsigned long long int *) (biggest_empty_block+8)) = bytes;
                        *(unsigned long long int*) (biggest_empty_block+bytes+16) = 0; 
                        *(unsigned long long int*) (biggest_empty_block+bytes+24)= oldSize-bytes-16;	
                }

                /* Retorna o espaço alocado de memória (pulando as quadwords) */
                return (biggest_empty_block+16);  		
        }

		/* passa para o proximo bloco somando com o tamanho do bloco atual + as 2 quadwords */

	/* Cria um novo bloco de bytes + 2 quadwords na heap */ 
	sbrk(bytes+16);

	/* brk_current aponta para o inicio do novo bloco, pulando as 2 quadwords */
	brk_current += bytes + 16;

	/* Seta o Ind. de Uso e tamanho do novo bloco */
	*((unsigned long long int *) (brk_current- 16 - bytes)) = 1; 
	*((unsigned long long int *) (brk_current- 8 - bytes)) = bytes;

	/* Retorna o endereço apontado por brk_current */
	return brk_current - bytes;
}

int memory_free(void *pointer){

	/* Marca o bloco como não usado*/
	*((long long*) (pointer-16)) = 0;

	return 0;
}

void printHeap() {
        void *tmp_brk = original_brk;

        while (tmp_brk < brk_current) {
                printf("| %lld | %lld | <-- %lld --> |", *(lluint *)tmp_brk, *(lluint *)(tmp_brk + 8), *(lluint *)(tmp_brk + 8));
                tmp_brk += *(lluint *)(tmp_brk + 8) + 16;
        }
        printf("\n");
}

//int main() {
//
//        setup_brk();
//	void *initial_brk = original_brk;
//	void *pnt_1, *pnt_2, *pnt_3, *pnt_4;
//
//	pnt_1 = memory_alloc(100);
//	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
//        printHeap();
//
//	pnt_2 = memory_alloc(130);
//	printf("==>> ALOCANDO UM ESPAÇO DE 130 BYTES:\n");
//        printHeap();
//
//	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
//	memory_free(pnt_1);
//        printHeap();
//
//	printf("==>> DESALOCANDO UM ESPAÇO DE 130 BYTES:\n");
//	memory_free(pnt_2);
//        printHeap();
//
//	pnt_3 = memory_alloc(24);
//	printf("==>> ALOCANDO UM ESPAÇO DE 24 BYTES:\n");
//        printHeap();
//
//	pnt_4 = memory_alloc(90);
//	printf("==>> ALOCANDO UM ESPAÇO DE 90 BYTES:\n");
//        printHeap();
//
//	printf("==>> DESALOCANDO UM ESPAÇO DE 24 BYTES:\n");
//	memory_free(pnt_3);
//        printHeap();
//
//	printf("==>> DESALOCANDO UM ESPAÇO DE 90 BYTES:\n");
//	memory_free(pnt_4);
//        printHeap();
//
//        return 0;
//}
