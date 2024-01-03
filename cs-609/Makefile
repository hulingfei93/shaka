default: build

build:
	-rm -rf bin
	mkdir bin
	mkdir bin/src
	mkdir bin/src/core
	mkdir bin/src/index
	mkdir bin/src/server
	$(MAKE) -f src/Makefile

install:
	$(MAKE) -f src/Makefile install

clean:
	rm -rf bin
