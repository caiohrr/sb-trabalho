#include <stdio.h>
#include <unistd.h>

void *original_brk = 0, *brk_current = 0;

void setup_brk(){
	original_brk = sbrk(0);
	brk_current = original_brk;
}

void dismiss_brk(){
	brk_current = original_brk;
	brk(original_brk);
} 

void* memory_alloc(unsigned long int bytes){
 
	brk_current += bytes+32;
	brk(brk_current+bytes+32);
	
	*((unsigned long long int *) (brk_current-bytes-32)) = 1; 
	*((unsigned long long int *) (brk_current-bytes-16)) = bytes;

	printf("malloc with %lld bytes in %lld\n", *((unsigned long long int *) (brk_current-bytes-16)), *((unsigned long long int *) (brk_current-bytes-32)));

	 

	return brk_current;
}

//1. Procura bloco livre com tamanho igual ou maior que a requisição
//2. Se encontrar, marca ocupação, utiliza os bytes necessários do bloco, retornando o endereço correspondente
//3. Se não encontrar, abre espaço para um novo bloco

int memory_free(void *pointer);


int main(int argc, char const *argv[])
{
	setup_brk();
	memory_alloc(100);
	dismiss_brk();
	return 0;
}