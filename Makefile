CFLAGS= -lm -Wall -std=c99 -O3 

tuktuk: main.o pointers.o graph.o process.o search.o structs.o
	gcc $(CFLAGS) -o tuktuk main.o pointers.o graph.o process.o search.o structs.o

main.o: main.c process.h pointers.h graph.h search.h structs.h
	gcc $(CFLAGS) -c main.c

process.o: process.c process.h pointers.h
	gcc $(CFLAGS) -c process.c

graph.o: graph.c graph.h pointers.h
	gcc $(CFLAGS) -c graph.c

pointers.o: pointers.c
	gcc $(CFLAGS) -c pointers.c

search.o: search.c search.h structs.h pointers.h graph.h
	gcc $(CFLAGS) -c search.c

structs.o: structs.c structs.h pointers.h
	gcc $(CFLAGS) -c structs.c

clean:
	rm -f *.o *.~ tuktuk *.gch
