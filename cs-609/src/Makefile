CC = cc
CFLAGS = -pipe -O0 -W -Wall -Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Werror -g
LINK = $(CC)

CORE_DEPS = src/core/sse_core.h \
	src/core/sse_alloc.h \
	src/core/sse_files.h \
	src/core/sse_log.h \
	src/core/sse_string.h \
	src/core/sse_pool.h \
	src/core/sse_hash.h \
	src/core/sse_rbtree.h \
	src/core/sse_vector.h \
	src/core/lz4.h \
	src/core/sse_tstree.h \
	src/core/sse_heap.h

INDEX_DEPS = src/index/sse_index.h \
			 src/index/sse_indexer_config.h \
			 src/index/sse_tokenizer.h \
			 src/index/sse_stemmer.h \
			 src/index/sse_term_dict.h \
			 src/index/sse_index_file.h \
			 src/index/sse_index_merge.h

SERVER_DEPS = src/server/sse_server.h \
			  src/server/sse_server_config.h \
			  src/server/sse_server_process.h \
			  src/server/sse_server_search.h

CORE_INCS = -I src/core
INDEX_INCS = -I src/index
SERVER_INCS = -I src/server
ONION_INCS = -I /usr/local/include

CORE_OBJS =	bin/src/core/sse_alloc.o \
	bin/src/core/sse_files.o \
	bin/src/core/sse_log.o \
	bin/src/core/sse_string.o \
	bin/src/core/sse_pool.o \
	bin/src/core/sse_hash.o \
	bin/src/core/sse_rbtree.o \
	bin/src/core/sse_vector.o \
	bin/src/core/lz4.o \
	bin/src/core/sse_tstree.o \
	bin/src/core/sse_heap.o

INDEX_OBJS = bin/src/index/sse_indexer_config.o \
			 bin/src/index/sse_tokenizer.o \
			 bin/src/index/sse_stemmer.o \
			 bin/src/index/sse_term_dict.o \
			 bin/src/index/sse_index_file.o \
			 bin/src/index/sse_index_merge.o

SERVER_OBJS = bin/src/server/sse_server_config.o \
			  bin/src/server/sse_server_process.o \
			  bin/src/server/sse_server_search.o

build: bin/indexer bin/server

bin/indexer: bin/src/index/indexer.o $(CORE_OBJS) $(INDEX_OBJS)
	$(LINK) -o bin/indexer bin/src/index/indexer.o $(CORE_OBJS) $(INDEX_OBJS) -lm

bin/src/index/indexer.o: $(CORE_DEPS) $(INDEX_DEPS) \
	src/index/indexer.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS) \
		-o bin/src/index/indexer.o \
		src/index/indexer.c

bin/server: bin/src/server/server.o $(CORE_OBJS) $(INDEX_OBJS) $(SERVER_OBJS)
	$(LINK) -o bin/server bin/src/server/server.o $(CORE_OBJS) $(INDEX_OBJS) $(SERVER_OBJS) -lonion -lm 

bin/src/server/server.o: $(CORE_DEPS) $(INDEX_DEPS) $(SERVER_DEPS) \
	src/server/server.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS) $(SERVER_INCS) $(ONION_INCS)\
		-o bin/src/server/server.o \
		src/server/server.c

bin/src/core/sse_alloc.o: $(CORE_DEPS) \
	src/core/sse_alloc.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_alloc.o \
		src/core/sse_alloc.c

bin/src/core/sse_files.o: $(CORE_DEPS) \
	src/core/sse_files.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_files.o \
		src/core/sse_files.c

bin/src/core/sse_log.o: $(CORE_DEPS) \
	src/core/sse_log.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_log.o \
		src/core/sse_log.c

bin/src/core/sse_string.o: $(CORE_DEPS) \
	src/core/sse_string.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_string.o \
		src/core/sse_string.c

bin/src/core/sse_pool.o: $(CORE_DEPS) \
	src/core/sse_pool.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_pool.o \
		src/core/sse_pool.c

bin/src/core/sse_hash.o: $(CORE_DEPS) \
	src/core/sse_hash.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_hash.o \
		src/core/sse_hash.c

bin/src/core/sse_rbtree.o: $(CORE_DEPS) \
	src/core/sse_rbtree.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_rbtree.o \
		src/core/sse_rbtree.c

bin/src/core/sse_vector.o: $(CORE_DEPS) \
	src/core/sse_vector.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_vector.o \
		src/core/sse_vector.c

bin/src/core/lz4.o: $(CORE_DEPS) \
	src/core/lz4.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/lz4.o \
		src/core/lz4.c

bin/src/core/sse_tstree.o: $(CORE_DEPS) \
	src/core/sse_tstree.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_tstree.o \
		src/core/sse_tstree.c

bin/src/core/sse_heap.o: $(CORE_DEPS) \
	src/core/sse_heap.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o bin/src/core/sse_heap.o \
		src/core/sse_heap.c

bin/src/index/sse_indexer_config.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_indexer_config.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_indexer_config.o \
		src/index/sse_indexer_config.c

bin/src/index/sse_tokenizer.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_tokenizer.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_tokenizer.o \
		src/index/sse_tokenizer.c

bin/src/index/sse_stemmer.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_stemmer.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_stemmer.o \
		src/index/sse_stemmer.c

bin/src/index/sse_term_dict.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_term_dict.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_term_dict.o \
		src/index/sse_term_dict.c

bin/src/index/sse_index_file.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_index_file.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_index_file.o \
		src/index/sse_index_file.c

bin/src/index/sse_index_merge.o: $(CORE_DEPS) $(INDEX_DEPS)\
	src/index/sse_index_merge.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS)\
		-o bin/src/index/sse_index_merge.o \
		src/index/sse_index_merge.c

bin/src/server/sse_server_config.o: $(CORE_DEPS) $(INDEX_DEPS) $(SERVER_DEPS)\
	src/server/sse_server_config.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS) $(SERVER_INCS)\
		-o bin/src/server/sse_server_config.o \
		src/server/sse_server_config.c

bin/src/server/sse_server_process.o: $(CORE_DEPS) $(INDEX_DEPS) $(SERVER_DEPS)\
	src/server/sse_server_process.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS) $(SERVER_INCS)\
		-o bin/src/server/sse_server_process.o \
		src/server/sse_server_process.c

bin/src/server/sse_server_search.o: $(CORE_DEPS) $(INDEX_DEPS) $(SERVER_DEPS)\
	src/server/sse_server_search.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) $(INDEX_INCS) $(SERVER_INCS)\
		-o bin/src/server/sse_server_search.o \
		src/server/sse_server_search.c
