CC = cc
CFLAGS = -pipe -O0 -W -Wall -Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Werror -g
LINK = $(CC)

DEPS = ls_core.h \
	   ls_alloc.h \
	   ls_log.h \
	   ls_pool.h \
	   ls_list.h \
	   ls_process.h \
	   ls_file.h \
	   ls_vector.h \
	   ls_print.h

INCS = -I ./

OBJS = ls_alloc.o \
	   ls_log.o \
       ls_pool.o \
	   ls_list.o \
	   ls_process.o \
	   ls_file.o \
	   ls_vector.o \
	   ls_print.o

default: build

build: ls

clean:
	rm -rf *.o
	rm -rf ls


ls: ls.o $(OBJS)
	$(LINK) -o ls ls.o $(OBJS) -lbsd -lm

ls.o: ls.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls.o ls.c

ls_alloc.o: ls_alloc.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_alloc.o ls_alloc.c

ls_log.o: ls_log.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_log.o ls_log.c

ls_pool.o: ls_pool.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_pool.o ls_pool.c

ls_list.o: ls_list.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_list.o ls_list.c

ls_process.o: ls_process.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_process.o ls_process.c

ls_file.o: ls_file.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_file.o ls_file.c
	
ls_vector.o: ls_vector.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_vector.o ls_vector.c

ls_print.o: ls_print.c $(DEPS)
	$(CC) -c $(CFLAGS) $(INCS) -o ls_print.o ls_print.c

