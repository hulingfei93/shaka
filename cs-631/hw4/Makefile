CC = cc
CFLAGS = -O0 -Wall -Werror -g
LINK = $(CC)

default: build

build: aed

clean:
	rm -rf *.o
	rm -rf aed

aed: aed.o
	$(LINK) -o aed aed.o -lbsd -lssl -lcrypto

aed.o: aed.c
	$(CC) -c $(CFLAGS) -o aed.o aed.c

