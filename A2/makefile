CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
#CFLAGS = -O

parser: parser.o error.o parser_utils.o

parser.o: parser.c apue.h

parser_utils.o: parser_utils.c apue.h

error.o: error.c apue.h

clean:
	rm -f *.o core parser cards.txt

run:
	./parser