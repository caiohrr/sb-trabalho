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

        int alloc_size = 128;

        void *ptr1 = memory_alloc(alloc_size);
        //printf("ptr1 = memory_alloc(128)\n");
        //printHeap();

        //printf("ptr1 = %llu\n", (long long unsigned int*)ptr1);

        void *ptr2 = memory_alloc(alloc_size * 4);
        //printf("ptr2 = memory_alloc(512)\n");
        //printHeap();

        //printf("ptr2 = %llu\n", (long long unsigned int *)ptr2);

        int *ptr = (int *)ptr1;

        for (int i = 0; i < alloc_size / sizeof(int); i++) {
                ptr[i] = i + 1;
        }

        //printf("Uso = %lld, Bytes = %lld\n", *(long long *)(ptr1 - 16), *(long long *)(ptr1 - 8));

        //for (int i = 0; i < alloc_size / sizeof(int); i++) {
        //        printf("%d, ", ptr[i]);
        //}


        //printf("\n(ptr1)Uso = %lld, Bytes = %lld\n", *(long long *)(ptr1 - 16), *(long long *)(ptr1 - 8));


        int ret = memory_free(ptr1);
        //printf("memory_free(ptr1)\n");
       //printHeap();


        //printf("\n(ptr1, depois do free)Uso = %lld, Bytes = %lld\n", *(long long *)(ptr1 - 16), *(long long *)(ptr1 - 8));

        void *ptr3 = memory_alloc(69);
        //printf("ptr2 = memory_alloc(69)\n");
        printHeap();

        
        //printf("\noriginal_brk = %lld, current_brk = %lld\n", (long long *)original_brk, (long long *)current_brk);

        //printf("\n(ptr1, depois do alloc)Uso = %lld, Bytes = %lld\n", *(long long *)(ptr1 - 16), *(long long *)(ptr1 - 8));
        //printf("ptr1 = %llu\n", (long long unsigned int*)ptr1);

        //printf("\n(ptr3, depois do alloc)Uso = %lld, Bytes = %lld\n", *(long long *)(ptr3 - 16), *(long long *)(ptr3 - 8));
        //printf("ptr3 = %llu\n", (long long unsigned int*)ptr3);



//        printf("\n(ptr2)Uso = %lld, Bytes = %lld\n", *(long long *)(ptr2 - 16), *(long long *)(ptr2 - 8));
//
//        printf("\nRet = %d\n", ret);
//
        return 0;
}
