CC := gcc
CFLAGS := -no-pie -g

rotina: memalloc.s novaRotina.c 
	$(CC) memalloc.s novaRotina.c $(CFLAGS) -o rotina

teste: memalloc.s teste.c
	$(CC) memalloc.s teste.c $(CFLAGS) -o teste

clean:
	rm -f teste rotina
