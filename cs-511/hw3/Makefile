CC = cc
CFLAGS = -Wall -pedantic-errors
LINK = $(CC)

INCS = -I ./

default: build

build: trafficmgr

clean:
	rm -rf *.o
	rm -rf trafficmgr


trafficmgr: trafficmgr.o monitor.o
	$(LINK) -o trafficmgr trafficmgr.o monitor.o -lpthread

trafficmgr.o: trafficmgr.c monitor.h
	$(CC) -c $(CFLAGS) $(INCS) -o trafficmgr.o trafficmgr.c

monitor.o: monitor.c monitor.h
	$(CC) -c $(CFLAGS) $(INCS) -o monitor.o monitor.c

