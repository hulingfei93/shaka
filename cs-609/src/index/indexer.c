#include <sse_core.h>
#include <sse_index.h>

void usage()
{
	printf("Usage: indexer <docdir> <indexdir> <tempdir> <stopwords_file> [-d].\n");
}

int 
main(int argc, char **argv)
{
	if (argc < 5) {
		usage();
		return 1;
	}

	sse_indexer_config_init(argv[1], argv[2], argv[3], argv[4]);
	if (argc > 5 && sse_strcmp(argv[5], "-d") == 0) {
		CONFIG.MODE = SSE_MODE_DEBUG;
	}
	
	/* create log */
	sse_log_t *log;
	if (CONFIG.MODE == SSE_MODE_DEBUG) {
		log = sse_log_init(SSE_NULL, SSE_NULL, CONFIG.LOG_LEVEL);
	}
	else {
		log = sse_log_init(CONFIG.LOG_DIR, CONFIG.LOG_NAME, CONFIG.LOG_LEVEL);
	}

	/* tokenization */
	sse_pool_t *tokenizer_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, log);
	sse_index_build_counter_t index_build_counter;
	sse_tokenizer_t *tokenizer = sse_tokenizer_create(&index_build_counter, tokenizer_pool, log);
	sse_tokenizer_execute(tokenizer);
	sse_pool_destory(tokenizer_pool);

	/* index merge */
	sse_pool_t *merge_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, log);
	sse_index_merger_t *merger = sse_index_merger_create(1, index_build_counter.index_file_count, merge_pool, log);
	sse_index_merge(merger);
	sse_pool_destory(merge_pool);

	return 0;
}
