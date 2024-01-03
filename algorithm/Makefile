CC = cc
CFLAGS = -pipe -O0 -W -Wall -Wpointer-arith -Wno-unused-parameter -Wno-unused-function -g
LINK = $(CC)

DEPS = src/core.h

INCS = -I src

default: build

build: prepare bin/math_test bin/array_test bin/binary_search_test bin/divide_and_conquer_test

prepare: 
	rm -rf bin
	mkdir bin
	mkdir bin/src
	mkdir bin/test

clean:
	rm -rf bin


# math
bin/math_test: bin/test/math_test.o bin/src/math.o
	$(LINK) -o bin/math_test bin/test/math_test.o bin/src/math.o -lm

bin/test/math_test.o: $(DEPS) test/math_test.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/test/math_test.o test/math_test.c

bin/src/math.o: $(DEPS) src/math.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/src/math.o src/math.c

# array
bin/array_test: bin/test/array_test.o bin/src/array.o
	$(LINK) -o bin/array_test bin/test/array_test.o bin/src/array.o -lm

bin/test/array_test.o: $(DEPS) test/array_test.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/test/array_test.o test/array_test.c

bin/src/array.o: $(DEPS) src/array.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/src/array.o src/array.c

# binary search
bin/binary_search_test: bin/test/binary_search_test.o bin/src/binary_search.o
	$(LINK) -o bin/binary_search_test bin/test/binary_search_test.o bin/src/binary_search.o -lm

bin/test/binary_search_test.o: $(DEPS) test/binary_search_test.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/test/binary_search_test.o test/binary_search_test.c

bin/src/binary_search.o: $(DEPS) src/binary_search.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/src/binary_search.o src/binary_search.c

# divide and conquer
bin/divide_and_conquer_test: bin/test/divide_and_conquer_test.o bin/src/divide_and_conquer.o
	$(LINK) -o bin/divide_and_conquer_test bin/test/divide_and_conquer_test.o bin/src/divide_and_conquer.o -lm

bin/test/divide_and_conquer_test.o: $(DEPS) test/divide_and_conquer_test.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/test/divide_and_conquer_test.o test/divide_and_conquer_test.c

bin/src/divide_and_conquer.o: $(DEPS) src/divide_and_conquer.c
	$(CC) -c $(CFLAGS) $(INCS) \
		-o bin/src/divide_and_conquer.o src/divide_and_conquer.c
		
