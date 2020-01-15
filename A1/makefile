CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
# CFLAGS = -O
CFLAGS += -pthread
# CFLAGS += -std=c11

LDLIBS += -lpthread
LDLIBS += -lrt
# LDLIBS += -lm  # used with math.h

a1: a1.o array.o error.o utils.o linked.o

a1.o: a1.c array.h apue.h linked.h

array.o: array.c apue.h utils.h

linked.o: linked.c apue.h utils.h array.h

error.o: error.c apue.h

utils.o: utils.c apue.h

clean:
	rm -f *.o a1 *.csv core

run:
	./a1