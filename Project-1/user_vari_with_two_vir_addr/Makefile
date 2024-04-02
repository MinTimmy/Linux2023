CC=gcc
CFLAGS=-c -Wall

main.out: main.o src.o
	$(CC) main.o src.o -o main.out

main.o: main.c
	$(CC) $(CFLAGS) main.c

src.o: src.c
	$(CC) $(CFLAGS) src.c

test.out: test.c
	$(CC) test.c -o test.out

__phys_addr_nodebug.out: __phys_addr_nodebug.c
	$(CC) __phys_addr_nodebug.c -o __phys_addr_nodebug.out
clean:
	rm -rf *.o *.out