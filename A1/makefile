CC=gcc
CFLAGS=-g -Wall -Werror -pedantic
#CFLAGS=-O

a1: a1.o array.o linked.o

a1.o: array.h linked.h

utils.o: utils.h

array: array.o utils.o

array.o: array.h utils.h

linked.o: linked.h

clean:
	rm -f *.o core a1

run:
	a1