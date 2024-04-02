CC=gcc
CFLAGS=-c -Wall
SRC_LOCATION=./../src

main.out: main.o src.o
	$(CC) main.o src.o -o main.out
	
main.o: main.c
	$(CC) $(CFLAGS) main.c

src.o: $(SRC_LOCATION)/src.c
	$(CC) $(CFLAGS) $(SRC_LOCATION)/src.c

partner.out: $(SRC_LOCATION)/partner.c
	$(CC) $(SRC_LOCATION)/partner.c -o partner.out

clean:
	rm -rf *.o *.out