CC=gcc
CFLAGS=-c -Wall
SRC_LOCATION=./../src


thread.out: thread.o src.o
	$(CC) thread.o src.o -o thread.out

thread.o: thread.c
	$(CC) $(CFLAGS) thread.c

src.o: src.c
	$(CC) $(CFLAGS) src.c

# partner.out: $(SRC_LOCATION)/partner.c
# 	$(CC) $(SRC_LOCATION)/partner.c -o partner.out

# lab.out: lab.c
# 	$(CC) lab.c -o lab.out
clean:
	rm -rf *.out *.o *txt