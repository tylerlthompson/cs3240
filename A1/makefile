CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
CFLAGS += -pthread
# CFLAGS = -std=c11

LDLIBS += -lpthread
LDLIBS += -lrt

a1: a1.o array.o error.o

a1.o: a1.c array.h apue.h

array.o: array.c apue.h

error.o: error.c apue.h

clean:
	rm -f *.o a1 *.csv

run:
	./a1