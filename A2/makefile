CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
# CFLAGS = -O
# CFLAGS += -pthread
# CFLAGS += -std=c11

# LDLIBS += -lpthread
# LDLIBS += -lrt
# LDLIBS += -lm  # used with math.h

parser: parser.o error.o

parser.o: parser.c apue.h

error.o: error.c apue.h

# utils.o: utils.c apue.h

clean:
	rm -f *.o *.csv core parser

run:
	./a1