#include <sse_core.h>
#include <sse_server.h>

static void sse_server_init_term_dict(sse_server_t *server);
static void sse_server_term_dict_check(sse_server_t *server);
static void sse_server_stopwords_filter_init(sse_server_t *server);
static void sse_server_init_doc(sse_server_t *server);

sse_server_t *
sse_server_create(sse_pool_t *pool, sse_log_t *log)
{
	sse_server_t *server;

	server = (sse_server_t *)sse_pool_alloc(pool, sizeof(sse_server_t));
	if (!server) {
		return SSE_NULL;
	}

	server->doc_count = 0;
	server->tstree_term_dict = sse_tstree_create(pool, log);
	server->stopwords_dict = sse_hash_create(SSE_TOKENIZER_STOPWORDS_HASHTABLE_SIZE, pool, log);
	server->pool = pool;
	server->log = log;

	return server;
}

void
sse_server_init(sse_server_t *server)
{
	sse_server_init_term_dict(server);
	/* sse_server_term_dict_check(server); */
	sse_server_stopwords_filter_init(server);

	sse_server_init_doc(server);

}

static void
sse_server_init_doc(sse_server_t *server)
{
	char							doc_index_path[SSE_PATH_MAX_SIZE];
	sse_index_file_doc_index_t		*doc_index;

	sse_file_path_fill(doc_index_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);
	doc_index = sse_index_file_doc_index_init(server->pool, server->log);
	sse_index_file_doc_index_open(doc_index, doc_index_path, "r");
	
	server->doc_count = doc_index->doc_count;

	sse_index_file_doc_index_close(doc_index);
}

static void
sse_server_init_term_dict(sse_server_t *server)
{
	char									term_dict_index_path[SSE_PATH_MAX_SIZE];
	char									term_dict_path[SSE_PATH_MAX_SIZE];
	sse_index_file_term_dict_index_t		*term_dict_index;
	sse_index_file_term_dict_t				*term_dict;
	sse_index_file_term_t					*term;
	sse_index_term_data_t					*term_data;
	sse_pool_t								*temp_pool;
	sse_vector_t							*stack;
	sse_index_term_dict_range_t				term_dict_low_range, term_dict_high_range, *temp;
	sse_uint_t								low, mid, high;

	temp_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, server->log);

	sse_file_path_fill(term_dict_index_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_TERM_DICT_INDEX_FILE_EXTENSION);
	sse_file_path_fill(term_dict_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_TERM_DICT_FILE_EXTENSION);

	term_dict_index = sse_index_file_term_dict_index_init(temp_pool, server->log);
	term_dict = sse_index_file_term_dict_init(temp_pool, server->log);

	sse_index_file_term_dict_index_open(term_dict_index, term_dict_index_path, "r");
	sse_index_file_term_dict_open(term_dict, term_dict_path, "r");

	/* create stack for building balanced ternary search tree */
	stack = sse_vector_create(sizeof(sse_index_term_dict_range_t), SSE_TERM_DICT_STACK_SIZE, temp_pool, server->log);

	if (term_dict->term_count == 0) {
		return;
	}

	term_dict_low_range.low = 0;
	term_dict_low_range.high = term_dict->term_count - 1;
	sse_vector_push_back(stack, &term_dict_low_range);

	while (!sse_vector_is_empty(stack)) {
		temp = (sse_index_term_dict_range_t *)sse_vector_top(stack);
		low = temp->low;
		high = temp->high;

		sse_vector_pop(stack);

		/* insert term into ternary search tree */
		mid = low + (high - low) / 2;
		term = sse_index_file_term_dict_random_read(term_dict, term_dict_index, mid);
		term_data = (sse_index_term_data_t *)sse_pool_alloc(server->pool, sizeof(sse_index_term_data_t));
		term_data->doc_freq = term->doc_freq;
		term_data->offset = term->offset;

		sse_tstree_insert(server->tstree_term_dict, &term->term, term_data);

		/* high range */
		if (mid + 1 <= high) {
			term_dict_high_range.low = mid + 1;
			term_dict_high_range.high = high;
			sse_vector_push_back(stack, &term_dict_high_range);
		}

		/* low range */
		if (mid > 0 && mid - 1 >= low) {
			term_dict_low_range.low = low;
			term_dict_low_range.high = mid - 1;
			sse_vector_push_back(stack, &term_dict_low_range);
		}
	}

	/* release resources */
	sse_index_file_term_dict_index_close(term_dict_index);
	sse_index_file_term_dict_close(term_dict);
	sse_pool_destory(temp_pool);
}

static void
sse_server_term_dict_check(sse_server_t *server)
{
	char									term_dict_path[SSE_PATH_MAX_SIZE];
	sse_index_file_term_dict_t				*term_dict;
	sse_index_file_term_t					*term;
	sse_index_term_data_t					*term_data;
	sse_pool_t								*temp_pool;
	sse_uint_t								term_match_count, term_no_match_count;
	sse_uint_t								doc_freq_match_count, doc_freq_no_match_count;
	sse_uint_t								offset_match_count, offset_no_match_count;
	
	temp_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, server->log);
	
	sse_file_path_fill(term_dict_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_TERM_DICT_FILE_EXTENSION);
	term_dict = sse_index_file_term_dict_init(temp_pool, server->log);
	sse_index_file_term_dict_open(term_dict, term_dict_path, "r");

	term_match_count = term_no_match_count = 0;
	doc_freq_match_count = doc_freq_no_match_count = 0;
	offset_match_count = offset_no_match_count = 0;
	
	printf("*****************term dict checking*********************\n");

	while ((term = sse_index_file_term_dict_read(term_dict)) != SSE_NULL) {
		term_data = sse_tstree_search(server->tstree_term_dict, &term->term);
		if (term_data == SSE_NULL) {
			++term_no_match_count;	
			continue;
		}
		else {
			++term_match_count;
			write(SSE_FD_STDOUT, term->term.data, term->term.len);
			write(SSE_FD_STDOUT, "\n", 1);
		}
		if (term_data->doc_freq != term->doc_freq) {
			++doc_freq_no_match_count;
		}
		else {
			++doc_freq_match_count;
		}
		if (term_data->offset != term->offset) {
			++offset_no_match_count;
		}
		else {
			++offset_match_count;
			/* printf("%d\t", (sse_uint_t)term_data->offset); */
		}
	}
	printf("term-%d-%d\n", term_match_count, term_no_match_count);
	printf("docfreq-%d-%d\n", doc_freq_match_count, doc_freq_no_match_count);
	printf("offset-%d-%d\n", offset_match_count, offset_no_match_count);

	/* release resources */
	sse_index_file_term_dict_close(term_dict);
	sse_pool_destory(temp_pool);
}

static void
sse_server_stopwords_filter_init(sse_server_t *server)
{
	FILE		*sw_fp;
	char		buf[SSE_TOKENIZER_STOPWORD_MAX_SIZE];
	size_t		buf_size, len;
	sse_str_t	*stopword;

	sw_fp = fopen(SERVER_CONFIG.STOPWORDS_FILE, "r");
	buf_size = SSE_TOKENIZER_STOPWORD_MAX_SIZE;

	if (sw_fp == SSE_NULL) {
		sse_log(SSE_LOG_ERROR, server->log, SSE_BOOL_TRUE, "File open failure: %s.", SERVER_CONFIG.STOPWORDS_FILE);
		return;
	}

	while (fgets(buf, buf_size, sw_fp) != SSE_NULL) {
		/* create a new stopword of sse_str_t obj */
		stopword = (sse_str_t *)sse_pool_alloc(server->pool, sizeof(sse_str_t));
		len = sse_strlen(buf);

		/* omit the LF at the end of string */
		if (buf[len - 1] == '\n') {
			--len;
		}
		stopword->len = len;
		stopword->data = (char *)sse_pool_alloc(server->pool, stopword->len);
		sse_memcpy(stopword->data, buf, stopword->len);

		sse_hash_add(server->stopwords_dict, stopword, SSE_NULL);
	}

	fclose(sw_fp);
}
