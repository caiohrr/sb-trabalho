CC := gcc
CFLAGS := -no-pie -g

rotina: memalloc.s rotinaDeTestes.c 
	$(CC) memalloc.s rotinaDeTestes.c $(CFLAGS) -o rotina

teste: memalloc.s teste.c
	$(CC) memalloc.s teste.c $(CFLAGS) -o teste

clean:
	rm teste rotina
