CC=gcc
CFLAGS=-c -Wall





project-1.out: project-1.o function.o
	gcc project-1.o function.o -o project.out

project-1.o: project-1.c
	$(CC) $(CFLAGS) project-1.c -o project-1.o

function.o: function.c
	$(CC) $(CFLAGS) function.c -o function.o

test.out: test.c
	$(CC) test.c -o test.out

clean:
	rm -r *o *out
