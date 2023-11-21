#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "meuAlocador.h"

void setup_brk(){
	original_brk = sbrk(0);
	brk_current = original_brk;
}

void dismiss_brk(){
	brk_current = original_brk;
	brk(original_brk);
} 

void* memory_alloc(unsigned long int bytes){
	void *tmp_brk = original_brk; 

	/* Laço para analisar os valores contidos na Heap. Para quando tmp_brk for maior q
	brk_current*/
	while((unsigned long long)tmp_brk < (unsigned long long)brk_current){

		/* Analisa se o bloco esta em uso e se o tamanho do bloco é maior/igual aos bytes pedidos */
		if(!*((unsigned long long int *)tmp_brk) && (*((unsigned long long int *)(tmp_brk+8)) >= bytes)){

			/* Marca o bloco como em uso */
			*((unsigned long long int *) (tmp_brk)) = 1; 

			/* Caso o tamanho do bloco atual menos os bytes pedidos e o tamanho das 2 quadwords for maior que 1
			cria um segundo bloco */ 
			if(*((unsigned long long int *)(tmp_brk+8))-bytes-16 >= 1){
				unsigned long long int oldSize = *((unsigned long long int*) (tmp_brk+8));

				*((unsigned long long int *) (tmp_brk+8)) = bytes;
				*(unsigned long long int*) (tmp_brk+bytes+16) = 0; 
				*(unsigned long long int*) (tmp_brk+bytes+24)= oldSize-bytes-16;	
				
			}

			/* Retorna o espaço alocado de memória (pulando as quadwords) */
			return (tmp_brk+16);  		
		}

		/* passa para o proximo bloco somando com o tamanho do bloco atual + as 2 quadwords */
		tmp_brk += *((unsigned long long int*)(tmp_brk+8))+16;
	}

	/* Cria um novo bloco de bytes + 2 quadwords na heap */ 
	sbrk(bytes+16);

	/* brk_current aponta para o inicio do novo bloco, pulando as 2 quadwords */
	brk_current += 16;

	/* Seta o Ind. de Uso e tamanho do novo bloco */
	*((unsigned long long int *) (brk_current-16)) = 1; 
	*((unsigned long long int *) (brk_current-8)) = bytes;

	/* Retorna o endereço apontado por brk_current */
	return brk_current;
}

int memory_free(void *pointer){

	/* Marca o bloco como não usado*/
	*((long long*) (pointer-16)) = 0;

	return 0;
}
