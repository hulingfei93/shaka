CC = cc
CFLAGS = -pipe -O0 -W -Wall -Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Werror -g
LINK = $(CC)

DEPS = sish_core.h \
       sish_log.h \
       sish_alloc.h \
       sish_pool.h \
       sish_vector.h \
       sish_parser.h \
       sish_process.h

INCS = -I ./

OBJS = sish_log.o \
       sish_alloc.o \
       sish_pool.o \
       sish_vector.o \
       sish_parser.o \
       sish_process.o

default: build

build: sish

clean:
    rm -rf *.o
    rm -rf sish


sish: sish.o $(OBJS)
    $(LINK) -o sish sish.o $(OBJS)

sish.o: sish.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish.o sish.c

sish_log.o: sish_log.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_log.o sish_log.c

sish_alloc.o: sish_alloc.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_alloc.o sish_alloc.c

sish_pool.o: sish_pool.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_pool.o sish_pool.c
    
sish_vector.o: sish_vector.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_vector.o sish_vector.c

sish_parser.o: sish_parser.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_parser.o sish_parser.c

sish_process.o: sish_process.c $(DEPS)
    $(CC) -c $(CFLAGS) $(INCS) -o sish_process.o sish_process.c

