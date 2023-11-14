#include <stdio.h>

extern void *memory_alloc(long long bytes);

extern void setup_brk();

extern int memory_free(void *ptr);

extern void *original_brk;

extern void *current_brk;

int main() {

        setup_brk();

        int alloc_size = 50;
        void *ptr1 = memory_alloc(alloc_size);
        int *ptr = (int *)ptr1;

        for (int i = 0; i < alloc_size / sizeof(int); i++) {
                ptr[i] = i + 1;
        }

        printf("Uso = %lld, Bytes = %lld\n", *(long long *)(ptr1 - 16), *(long long *)(ptr1 - 8));

        for (int i = 0; i < alloc_size / sizeof(int); i++) {
                printf("%d, ", ptr[i]);
        }

        int ret = memory_free(ptr);
        printf("\nRet = %d\n", ret);

        return 0;
}
