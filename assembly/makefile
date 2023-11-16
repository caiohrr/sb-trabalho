CC := gcc
CFLAGS := -no-pie -g

teste: memalloc.s teste.c
	$(CC) memalloc.s teste.c $(CFLAGS) -o teste

clean:
	rm teste
