#include <stdio.h>

extern void *memory_alloc(long long bytes);

extern void setup_brk();

extern int memory_free(void *ptr);

extern void *original_brk;

extern void *current_brk;

#define lluint long long unsigned int

void printHeap() {
        void *tmp_brk = original_brk;

        while (tmp_brk < current_brk) {
                printf("| %lld | %lld | <-- %lld --> |", *(lluint *)tmp_brk, *(lluint *)(tmp_brk + 8), *(lluint *)(tmp_brk + 8));
                tmp_brk += *(lluint *)(tmp_brk + 8) + 16;
        }
        printf("\n");
}

int main() {

        setup_brk();

        printf("Alocando 100 bytes\n");
        void *ptr1 = memory_alloc(100);
        printHeap();
        printf("Desalocando 100 bytes\n");
        memory_free(ptr1);
        printHeap();

        printf("Alocando 50 bytes\n");
        void *ptr2 = memory_alloc(50);
        printHeap();
        printf("Alocando 100 bytes\n");
        void *ptr3 = memory_alloc(100);
        printHeap();


        printf("Desaloca 50 bytes\n");
        memory_free(ptr2);
        printHeap();

        printf("Desaloca 100 bytes\n");
        memory_free(ptr3);
        printHeap();


        return 0;
}
