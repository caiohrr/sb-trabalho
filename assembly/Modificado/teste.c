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
	void *initial_brk = original_brk;
	void *pnt_1, *pnt_2, *pnt_3, *pnt_4;

	pnt_1 = memory_alloc(100);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
        printHeap();

	pnt_2 = memory_alloc(130);
	printf("==>> ALOCANDO UM ESPAÇO DE 130 BYTES:\n");
        printHeap();

	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	memory_free(pnt_1);
        printHeap();

	printf("==>> DESALOCANDO UM ESPAÇO DE 130 BYTES:\n");
	memory_free(pnt_2);
        printHeap();

	pnt_3 = memory_alloc(24);
	printf("==>> ALOCANDO UM ESPAÇO DE 24 BYTES:\n");
        printHeap();

	pnt_4 = memory_alloc(90);
	printf("==>> ALOCANDO UM ESPAÇO DE 90 BYTES:\n");
        printHeap();

	printf("==>> DESALOCANDO UM ESPAÇO DE 24 BYTES:\n");
	memory_free(pnt_3);
        printHeap();

	printf("==>> DESALOCANDO UM ESPAÇO DE 90 BYTES:\n");
	memory_free(pnt_4);
        printHeap();

        return 0;
}
