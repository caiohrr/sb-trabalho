
CFLAGS=-lm -g -Wall

OBJECTS="meuAlocador.o"

testes:rotinaDeTestes.c meuAlocador.o 
	gcc rotinaDeTestes.c -o testes meuAlocador.o $(CFLAGS)

meuAlocador.o:meuAlocador.c
	gcc meuAlocador.c -c $(CFLAGS)

clean:
	rm -f testes $(OBJECTS)