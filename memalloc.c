#include <stdio.h>
#include <unistd.h>
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

                //current_mem_reg = *(MemoryRegister *) tmp_brk;

                //printMemReg(current_mem_reg);
                //printf("curr_brk = %p\n", current_brk);
                //printf("brk = %p\n", tmp_brk);
                //printf("brk+ = %p\n", tmp_brk + sizeof(MemoryRegister) + current_mem_reg.bytes);
                //return NULL;

                while ((unsigned long int) tmp_brk < (unsigned long int) current_brk) {
                        current_mem_reg = *(MemoryRegister *) tmp_brk;

                        printMemReg(current_mem_reg);
                        printf("brk = %p\n", tmp_brk);
                        // Bloco atual está em uso, se for o caso pula para o próximo
                        if (current_mem_reg.use == 1) {
                                //printf("YES\n");
                                tmp_brk += sizeof(MemoryRegister) + current_mem_reg.bytes;
                        } else {
                                // Bloco não está em uso e comporta a alocação atual
                                if (current_mem_reg.bytes > bytes) {

                                        printf("inside if\n");
                                        mem_reg.use = 1;
                                        mem_reg.bytes = bytes;
                                        *(MemoryRegister *) tmp_brk = mem_reg;
                                        void *return_brk = (tmp_brk + sizeof(MemoryRegister));

                                        mem_reg.use = 0;
                                        mem_reg.bytes = current_mem_reg.bytes - (bytes + sizeof(MemoryRegister));
                                        *(MemoryRegister *) ((unsigned long int) tmp_brk + bytes + sizeof(MemoryRegister)) = mem_reg;

                                        return return_brk;
                                }
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
        if ((unsigned long int) pointer > (unsigned long int) current_brk)
                return -1;

        //unsigned long int *ptr = (unsigned long int *) pointer;
        printf("\npointer = %p\n", pointer);
        printf("val = %ld | pointer - 2 = %p\n", *(unsigned long int *) (pointer - 16), pointer - 16);
        printf("val = %ld | pointer - 1 = %p\n", *(unsigned long int *) (pointer - 8), pointer - 8);

        *(unsigned long int *) (pointer - sizeof(MemoryRegister)) = 0;

        return 0;
}

int main() {

        setup_brk();
        
        void *ptr2 = memory_alloc(1024);
        void *ptr = memory_alloc(100);

        printf("%p, %p\n", ptr, ptr2);
        
        char *string = (char *) ptr;
        int *int_arr = (int *) ptr2;

        string[0] = 'P';
        string[1] = 'o';
        string[2] = 'g';
        string[3] = 'g';
        string[4] = 'e';
        string[5] = 'r';
        string[6] = 's';
        string[7] = '\0';

        for (int i = 0; i < 1024 / sizeof(int); i ++) {
                int_arr[i] = i + 1;
                printf("%d, ", int_arr[i]);
        }

        printf("\n%s\n", string);

        if (memory_free(ptr) == 0)
                printf("liberou com sucesso\n");

        void *ptr3 = memory_alloc(50);
        printf("%p", ptr3);
        char *string2 = (char *) ptr3;

        printf("\n%s\n", string);
        printf("\n%s\n", string2);

        return 0;
}
