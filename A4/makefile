CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
#CFLAGS = -O

defrag: defrag.o error.o

defrag.o: defrag.c apue.h

error.o: error.c apue.h

clean:
	rm -f *.o core defrag music.mp3

run:
	./defrag dirs