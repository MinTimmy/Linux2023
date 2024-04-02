CC=gcc
CFLAGS=-c -Wall
SRC_LOCATION=./../src

lab.out: lab.o src.o
	$(CC) lab.o src.o -o lab.out

lab.o: lab.c
	$(CC) $(CFLAGS) lab.c

src.o: $(SRC_LOCATION)/src.c
	$(CC) $(CFLAGS) $(SRC_LOCATION)/src.c

partner.out: $(SRC_LOCATION)/partner.c
	$(CC) $(SRC_LOCATION)/partner.c -o partner.out

clean:
	rm -rf *.out *.o