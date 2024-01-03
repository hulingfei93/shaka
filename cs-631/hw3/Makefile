CC = cc
CFLAGS = -pipe -O0 -W -Wall -Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Werror -g
LINK = $(CC)

DEPS = core.h \
	   net.h

INCS = -I ./

OBJS = net.o

default: build

build: sws

clean:
	rm -rf *.o
	rm -rf sws


sws: sws.o $(OBJS)
	$(LINK) -o sws sws.o $(OBJS) -lbsd -lpthread

sws.o: main.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o sws.o main.c

net.o: net.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o net.o net.c

