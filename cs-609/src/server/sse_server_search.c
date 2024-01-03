#include <sse_core.h>
#include <sse_server.h>

static void sse_server_search_tokenize(char *query_text, sse_server_search_query_t *query, sse_server_t *server);
static void sse_server_search_tokenizer_token_push(sse_server_search_query_tokenizer_t *tokenizer, char *address);
static void sse_server_search_tokenizer_token_fetch(sse_server_search_query_tokenizer_t *tokenizer, sse_server_search_query_t *query, sse_server_t *server);
static void sse_server_search_tokenizer_token_tolower(sse_str_t *token);
static void sse_server_search_compute_score(sse_server_search_query_t *query, sse_server_t *server);
static sse_id_t sse_server_search_find_smallest_docid(sse_server_search_query_t *query, sse_uint_t *indexes);
static void sse_server_search_posting_check(sse_server_search_query_t *query);
static void sse_server_search_top_docs_check(sse_server_search_query_t *query);


static void sse_server_search_compute_shortest_summary(sse_server_search_query_t *query);
static void sse_server_search_compute_shortest_summary_imp(sse_server_search_query_t *query, sse_vector_t *offset_matrix, sse_server_search_result_t *result, sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc);
static sse_bool_t sse_server_search_terms_all_exist(sse_uint_t *flags, sse_uint_t flag_len, sse_uint_t term_threshold);
static void sse_server_search_find_smallest_offset(sse_vector_t *offset_matrix, sse_uint_t *indexes, sse_off_t *smallest_offset, sse_uint_t *smallest_term_index);
static sse_index_file_posting_t * sse_server_search_find_offset(sse_server_search_query_t *query, sse_server_search_query_term_t *query_term, sse_id_t doc_id);
static void sse_server_search_shortest_summary_check(sse_server_search_query_t *query);

char *
sse_server_search_doc(sse_id_t doc_id, sse_server_t *server, sse_pool_t *pool, sse_log_t *log)
{
	char							doc_index_path[SSE_PATH_MAX_SIZE];
	char							doc_path[SSE_PATH_MAX_SIZE];
	sse_index_file_doc_index_t		*doc_index;
	sse_index_file_doc_t			*doc;
	char							*doc_content;				
	
	if (doc_id < 1 || doc_id > server->doc_count) {
		return SSE_NULL;
	}

	/* open index files */
	sse_file_path_fill(doc_index_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);
	doc_index = sse_index_file_doc_index_init(pool, log);
	sse_index_file_doc_index_open(doc_index, doc_index_path, "r");
	
	sse_file_path_fill(doc_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOC_DATA_FILE_EXTENSION);
	doc = sse_index_file_doc_init(pool, log);
	sse_index_file_doc_open(doc, doc_path, "r");

	doc_content = sse_index_file_doc_get(doc_index, doc, doc_id, pool);
	
	/* close index files */
	sse_index_file_doc_index_close(doc_index);
	sse_index_file_doc_close(doc);

	return doc_content;
}

sse_server_search_query_t *
sse_server_search (char *query_text, sse_uint_t top, sse_server_search_model_t search_model, sse_server_t *server, sse_pool_t *pool, sse_log_t *log)
{
	sse_server_search_query_t *query;
	/* create query */
	query = (sse_server_search_query_t *)sse_pool_alloc(pool, sizeof(sse_server_search_query_t));

	query->term_hash = sse_hash_create(SSE_QUERY_TERM_NORMAL_SIZE, pool, log);
	query->posting_list = sse_vector_create(sizeof(sse_server_search_query_term_t), SSE_QUERY_TERM_NORMAL_SIZE, pool, log);
	query->top_docs = sse_heap_create(top, pool, log);
	query->results_len = 0;
	query->search_model = search_model;
	query->pool = pool;
	query->log = log;	

	/* tokenize, lower case, filter stop words, stemming */
	sse_server_search_tokenize(query_text, query, server);

	/* compute score */
	sse_server_search_compute_score(query, server);

	//sse_server_search_posting_check(query);
	
	//sse_server_search_top_docs_check(query);

	/* compute shortest summary */
	sse_server_search_compute_shortest_summary(query);

	//sse_server_search_shortest_summary_check(query);

	return query; 
}

static void
sse_server_search_shortest_summary_check(sse_server_search_query_t *query)
{
	sse_uint_t						i;
	sse_server_search_result_t		*result;

	printf("*******query results***********\n");
	for (i = 0; i < query->results_len; ++i) {
		result = query->results + i;
		printf("%u---%u---%f---%s---%s\n", i, result->doc_id, result->score, result->title, result->summary);
	}
}

static void
sse_server_search_compute_shortest_summary(sse_server_search_query_t *query)
{
	char							offset_file_path[SSE_PATH_MAX_SIZE];
	char							doc_index_path[SSE_PATH_MAX_SIZE];
	char							doc_path[SSE_PATH_MAX_SIZE];
	sse_index_file_offset_t			*offset_file;
	sse_index_file_doc_index_t		*doc_index;
	sse_index_file_doc_t			*doc;
	sse_uint_t						i, j;
	sse_index_file_posting_t		*posting_data;
	sse_vector_t					*offset_matrix, *new_offsets;
	sse_vector_t					**offset_list;
	int *							doc_id;
	sse_server_search_query_term_t	*query_term;

	if (query->top_docs->size == 0) {
		return;
	}

	/* init */
	offset_matrix = sse_vector_create(sizeof(sse_vector_t *), query->posting_list->size, query->pool, query->log);
	for (i = 0; i < query->posting_list->size; ++i) {
		new_offsets = sse_vector_create(sizeof(sse_off_t), SSE_QUERY_OFFSET_NORMAL_SIZE, query->pool, query->log);
		sse_vector_push_back(offset_matrix, &new_offsets);
	}

	/* open index file */
	sse_file_path_fill(offset_file_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_OFFSET_FILE_EXTENSION);
	offset_file = sse_index_file_offset_init(query->pool, query->log);
	sse_index_file_offset_open(offset_file, offset_file_path, "r");

	sse_file_path_fill(doc_index_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);
	doc_index = sse_index_file_doc_index_init(query->pool, query->log);
	sse_index_file_doc_index_open(doc_index, doc_index_path, "r");
	
	sse_file_path_fill(doc_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOC_DATA_FILE_EXTENSION);
	doc = sse_index_file_doc_init(query->pool, query->log);
	sse_index_file_doc_open(doc, doc_path, "r");
	
	/* create results */
	query->results = (sse_server_search_result_t *)sse_pool_alloc(query->pool, sizeof(sse_server_search_result_t) * query->top_docs->size);
	query->results_len = query->top_docs->size;

	/* read offset info */
	for (i = 0; i < query->top_docs->size; ++i) {
		doc_id = (int *)query->top_docs->items[i].data;
		query->results[i].doc_id = *doc_id;
		query->results[i].score = query->top_docs->items[i].score;
		query->results[i].title = sse_index_file_doc_name_read(doc_index, doc, *doc_id, query->pool, &query->results[i].doc_size);

		/* read offset info */
		for (j = 0; j < query->posting_list->size; ++j) {
			/* for each doc id, find related offset info in every term */
			query_term = (sse_server_search_query_term_t *)sse_vector_get_at(query->posting_list, j);
			posting_data = sse_server_search_find_offset(query, query_term, *doc_id);
			if (posting_data != SSE_NULL) {
				/* store offset info */
				offset_list = (sse_vector_t **)sse_vector_get_at(offset_matrix, j);
				sse_index_file_offset_read(offset_file, posting_data->offset, posting_data->term_freq, *offset_list);
			}
		}

		/* compute shortest summary */
		sse_server_search_compute_shortest_summary_imp(query, offset_matrix, query->results + i, doc_index, doc);

		/* reset the offset info */
		for (j = 0; j < offset_matrix->size; ++j) {
			offset_list = (sse_vector_t **)sse_vector_get_at(offset_matrix, j);
			sse_vector_cleanup(*offset_list);
		}
	}

	/* close index files */
	sse_index_file_offset_close(offset_file);
	sse_index_file_doc_index_close(doc_index);
	sse_index_file_doc_close(doc);
}

static void
sse_server_search_compute_shortest_summary_imp(sse_server_search_query_t *query, sse_vector_t *offset_matrix, sse_server_search_result_t *result, sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc)
{
	sse_pool_t							*temp_pool;
	sse_uint_t							i, begin, end;
	sse_uint_t							term_threshold = 0;
	sse_vector_t						**offset_list;
	sse_off_t							smallest_offset;
	sse_uint_t							smallest_term_index;
	sse_uint_t							*indexes;
	sse_vector_t						*sorted_offset_list;
	sse_server_search_position_t		position;
	sse_server_search_position_t		*begin_pos, *end_pos;
	sse_uint_t							*flags;
	sse_off_t							shortest_offset_range;
	sse_uint_t							shortest_begin, shortest_end;

	/* init */
	temp_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, query->log);

	indexes = sse_pool_alloc(temp_pool, offset_matrix->size * sizeof(sse_uint_t));
	for (i = 0; i < offset_matrix->size; ++i) {
		indexes[i] = 0;
	}
	sorted_offset_list = sse_vector_create(sizeof(sse_server_search_position_t), SSE_QUERY_OFFSET_NORMAL_SIZE, temp_pool, query->log);
	flags = sse_pool_alloc(temp_pool, offset_matrix->size * sizeof(sse_uint_t));
	for (i = 0; i < offset_matrix->size; ++i) {
		flags[i] = 0;
	}

	/* compute threshold */
	for (i = 0; i < offset_matrix->size; ++i) {
		offset_list = (sse_vector_t **)sse_vector_get_at(offset_matrix, i);
		if (!sse_vector_is_empty(*offset_list)) {
			++term_threshold;
		}
	}

	if (term_threshold == 0) {
		return;
	}

	/* sort offset info */
	while(1) {
		sse_server_search_find_smallest_offset(offset_matrix, indexes, &smallest_offset, &smallest_term_index);
		if (smallest_offset == -1) {
			break;
		}

		position.term_index = smallest_term_index;
		position.offset = smallest_offset;
		sse_vector_push_back(sorted_offset_list, &position);
	}

	/************* compute shortest range *******************/

	/* find the first candidate shortest range */
	begin = end = 0;
	end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, end);
	++flags[end_pos->term_index];
	while(!sse_server_search_terms_all_exist(flags, offset_matrix->size, term_threshold)) {
		++end;
		end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, end);
		++flags[end_pos->term_index];
	}
	begin_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, begin);
	end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, end);
	/* update shorest range */
	shortest_begin = begin;
	shortest_end = end;
	shortest_offset_range = end_pos->offset - begin_pos->offset;

	/* find the other candidate shortest ranges */
	while (1) {
		if (sse_server_search_terms_all_exist(flags, offset_matrix->size, term_threshold)) {
			begin_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, begin);
			end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, end);
			
			/* update shorest range*/
			if (end_pos->offset - begin_pos->offset < shortest_offset_range)
			{
				shortest_begin = begin;
				shortest_end = end;
				shortest_offset_range = end_pos->offset - begin_pos->offset;
			}

			/* shrink the range*/
			--flags[begin_pos->term_index];
			++begin;
		}
		else {
			if (++end < sorted_offset_list->size) {
				end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, end);
				++flags[end_pos->term_index];
			}
			else {
				break;
			}
		}
	}

	/* generate shortest summary */
	begin_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, shortest_begin);
	end_pos = (sse_server_search_position_t *)sse_vector_get_at(sorted_offset_list, shortest_end);
	//printf("%u-%u-%u\n", (sse_uint_t)begin_pos->offset, (sse_uint_t)end_pos->offset, (sse_uint_t)shortest_offset_range);

	size_t		len, summary_len;
	sse_off_t	begin_offset, end_offset;
	const char	*ellipsis = "... ";
	size_t		ellipsis_len = sse_strlen(ellipsis);
	size_t		first_len, second_len;
	sse_off_t	first_begin_offset, first_end_offset;
	sse_off_t	second_begin_offset, second_end_offset;

	len = 0;
	summary_len = 0;
	if (begin_pos->offset == end_pos->offset || shortest_offset_range <= SSE_QUERY_RESULT_SUMMARY_MAX_SIZE) {
		if (begin_pos->offset <= SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE / 3) {
			begin_offset = 0;
		}
		else {
			begin_offset = begin_pos->offset - SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE / 3;
		}
		end_offset = end_pos->offset + SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE;
		len = end_offset - begin_offset;
			
		result->summary = (char *)sse_pool_calloc(query->pool, len + ellipsis_len + 1);
		summary_len = sse_index_file_doc_read(doc_index, doc, result->doc_id, begin_offset, result->summary, len);
		sse_strcpy(result->summary + summary_len, ellipsis);
	}
	else {
		if (begin_pos->offset <= SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE / 3) {
			first_begin_offset = 0;
		}
		else {
			first_begin_offset = begin_pos->offset - SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE / 3;
		}
		first_end_offset = begin_pos->offset + SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE;
		first_len = first_end_offset - first_begin_offset;

		second_begin_offset = end_pos->offset - SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE / 3;
		second_end_offset = end_pos->offset + SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE;
		second_len = second_end_offset - second_begin_offset;
			
		result->summary = (char *)sse_pool_calloc(query->pool, first_len + second_len + 2 * ellipsis_len + 1);
		summary_len = sse_index_file_doc_read(doc_index, doc, result->doc_id, first_begin_offset, result->summary, first_len);
		sse_strcpy(result->summary + summary_len, ellipsis);
		summary_len += ellipsis_len;

		summary_len += sse_index_file_doc_read(doc_index, doc, result->doc_id, second_begin_offset, result->summary + summary_len, second_len);
		sse_strcpy(result->summary + summary_len, ellipsis);
	}

	sse_pool_destory(temp_pool);
}

static sse_bool_t
sse_server_search_terms_all_exist(sse_uint_t *flags, sse_uint_t flag_len, sse_uint_t term_threshold)
{
	sse_uint_t		i, count;

	count = 0;
	for (i = 0; i < flag_len; ++i) {
		if (flags[i] > 0) {
			++count;
		} 
	}

	return (count == term_threshold) ? SSE_BOOL_TRUE : SSE_BOOL_FALSE; 
}

static void
sse_server_search_find_smallest_offset(sse_vector_t *offset_matrix, sse_uint_t *indexes, sse_off_t *smallest_offset, sse_uint_t *smallest_term_index)
{
	sse_uint_t							i;
	sse_vector_t						**offset_list;
	sse_off_t							*offset;

	*smallest_offset = -1;

	for (i = 0; i < offset_matrix->size; ++i) {
		offset_list = (sse_vector_t **)sse_vector_get_at(offset_matrix, i);
		if (indexes[i] < (*offset_list)->size) {
			offset = (sse_off_t *)sse_vector_get_at(*offset_list, indexes[i]);
			if (*smallest_offset == -1 || *smallest_offset > *offset) {
				*smallest_offset = *offset;
				*smallest_term_index = i;
			}
		}
	}
	if (*smallest_offset != -1) {
		++indexes[*smallest_term_index];
	}
}

static sse_index_file_posting_t *
sse_server_search_find_offset(sse_server_search_query_t *query, sse_server_search_query_term_t *query_term, sse_id_t doc_id)
{
	sse_vector_t						*posting_list;
	sse_index_file_posting_t			*posting_data;
	sse_uint_t							low, high, mid;

	posting_list = query_term->posting;

	if (posting_list->size == 0) {
		return SSE_NULL;
	}

	low = 0;
	high = posting_list->size - 1;

	while (low <= high) {
		mid = low + (high - low) / 2;
		posting_data = (sse_index_file_posting_t *)sse_vector_get_at(posting_list, mid);

		if (posting_data->doc_id < doc_id) {
 			low = mid + 1;
		}
		else if (posting_data->doc_id > doc_id) {
			if (mid == 0) {
				break;
			}
			high = mid - 1;
		}
		else {
			return posting_data;
		}
	}
	return SSE_NULL;
}

static void
sse_server_search_top_docs_check(sse_server_search_query_t *query)
{
	sse_uint_t		i;
	float			score;
	int				*doc_id;
	
	printf("*******top docs***********\n");
	for (i = 0; i < query->top_docs->size; ++i) {
		score = query->top_docs->items[i].score;
		doc_id = (int *)query->top_docs->items[i].data;
		printf("%f-%d\n", score, *doc_id);
	}
}

static void
sse_server_search_posting_check(sse_server_search_query_t *query)
{
	sse_uint_t i, j;
	sse_server_search_query_term_t	*query_term;
	sse_index_file_posting_t		*posting_data;

	printf("*******posting info***********\n");
	for (i = 0; i < query->posting_list->size; ++i) {
		query_term = (sse_server_search_query_term_t *)sse_vector_get_at(query->posting_list, i);
		write(SSE_FD_STDOUT, query_term->term.data, query_term->term.len);
		printf("-%d\n", query_term->doc_freq);
		fflush(SSE_STDIO_STDOUT);
		for (j = 0; j < query_term->posting->size; ++j) {
			posting_data = (sse_index_file_posting_t *)sse_vector_get_at(query_term->posting, j);
			printf("%d-%d-%f-%d", posting_data->doc_id, posting_data->term_freq, posting_data->weight, (sse_uint_t)posting_data->offset);
			printf("\t");
			fflush(SSE_STDIO_STDOUT);
		}
		printf("\n");
		fflush(SSE_STDIO_STDOUT);
	}
}

static void
sse_server_search_compute_score(sse_server_search_query_t *query, sse_server_t *server)
{
	char							docid_freq_path[SSE_PATH_MAX_SIZE];
	sse_index_file_docid_freq_t		*docid_freq;
	sse_uint_t						i;
	sse_server_search_query_term_t	*query_term;
	sse_index_term_data_t			*term_data;
	sse_uint_t						*temp_indexes;
	sse_id_t						smallest_doc_id;
	float							idf, idf_sum, score;
	sse_id_t						*doc_id;
	sse_index_file_posting_t		*posting_data;
	
	if (query->posting_list->size == 0) {
		return;
	}

	/* open docid freq index file */
	sse_file_path_fill(docid_freq_path, SERVER_CONFIG.INDEX_DIR, SERVER_CONFIG.INDEX_NAME, SERVER_CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION);
	docid_freq = sse_index_file_docid_freq_init(query->pool, query->log);
	sse_index_file_docid_freq_open(docid_freq, docid_freq_path, "r");

	/* read posting info */
	for (i = 0; i < query->posting_list->size; ++i) {
		query_term = (sse_server_search_query_term_t *)sse_vector_get_at(query->posting_list, i);
		query_term->doc_freq = 0;
		query_term->posting = sse_vector_create(sizeof(sse_index_file_posting_t), SSE_QUERY_POSTING_NORMAL_SIZE, query->pool, query->log);
		/* search posting info from tstree */
		term_data = sse_tstree_search(server->tstree_term_dict, &query_term->term);
		/* read docid freq info */
		if (term_data != SSE_NULL) {
			query_term->doc_freq = term_data->doc_freq;
			sse_index_file_docid_freq_read(docid_freq, term_data->offset, term_data->doc_freq, query_term->posting);
		}
	}
	/* close docid freq index file */
	sse_index_file_docid_freq_close(docid_freq);


	temp_indexes = sse_pool_alloc(query->pool, query->posting_list->size * sizeof(sse_uint_t));
	for (i = 0; i < query->posting_list->size; ++i) {
		temp_indexes[i] = 0;
	}

	while ((smallest_doc_id = sse_server_search_find_smallest_docid(query, temp_indexes)) != 0) {
		idf_sum = 0.0f;
		score = 0.0f;
		/* compute score for each doc */
		for (i = 0; i < query->posting_list->size; ++i) {
			query_term = (sse_server_search_query_term_t *)sse_vector_get_at(query->posting_list, i);
			if (temp_indexes[i] < query_term->posting->size) {
				posting_data = (sse_index_file_posting_t *)sse_vector_get_at(query_term->posting, temp_indexes[i]);
				if (smallest_doc_id ==  posting_data->doc_id) {
					++temp_indexes[i];
					idf = log10f(server->doc_count / (float)query_term->doc_freq);
					if (query->search_model == STANDARD_MODEL)
					{
						score += (idf * posting_data->weight);
					}
					else {
						score += (idf * idf * posting_data->term_freq);
					}
					idf_sum += (idf * idf);
				}
			}
		}
		if (idf_sum > 0) {
			score = score / sqrtf(idf_sum);
		}
		doc_id = (sse_id_t *)sse_pool_alloc(query->pool, sizeof(sse_id_t));
		*doc_id = smallest_doc_id;
		sse_heap_min_insert(query->top_docs, score, doc_id);
	}
	sse_heap_min_sort(query->top_docs);
}

static sse_id_t
sse_server_search_find_smallest_docid(sse_server_search_query_t *query, sse_uint_t *indexes)
{
	sse_id_t							smallest_doc_id = 0;
	sse_uint_t							i;
	sse_server_search_query_term_t		*query_term;
	sse_index_file_posting_t			*posting_data;

	for (i = 0; i < query->posting_list->size; ++i) {
		query_term = (sse_server_search_query_term_t *)sse_vector_get_at(query->posting_list, i);
		if (indexes[i] < query_term->posting->size) {
			posting_data = (sse_index_file_posting_t *)sse_vector_get_at(query_term->posting, indexes[i]);
			if (smallest_doc_id == 0 || smallest_doc_id > posting_data->doc_id) {
				smallest_doc_id = posting_data->doc_id;
			}
		}
	}

	return smallest_doc_id;
}

static void
sse_server_search_tokenize(char *query_text, sse_server_search_query_t *query, sse_server_t *server) 
{
	char									*s;
	sse_server_search_query_tokenizer_t		tokenizer;

	SSE_QUERY_TOKEN_EMPTY(&tokenizer);
	s = query_text;
	while(*s) {
		if (isprint(*s)) {
			if (isalpha(*s)) {	/* a-z A-Z */
				if (SSE_QUERY_TOKEN_FLAGS_HAVE_COMMA(&tokenizer)
							|| SSE_QUERY_TOKEN_FLAGS_HAVE_PERIOD(&tokenizer)) {
					sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
				}
				sse_server_search_tokenizer_token_push(&tokenizer, s);
				SSE_QUERY_TOKEN_FLAGS_SET_ALPHA(&tokenizer);
			}
			else if (isdigit(*s)) { /* 0-9 */
				sse_server_search_tokenizer_token_push(&tokenizer, s);
				SSE_QUERY_TOKEN_FLAGS_SET_DIGIT(&tokenizer);
			}
			else if (*s == '-') {
				if (SSE_QUERY_TOKEN_FLAGS_HAVE_ALPHA(&tokenizer)
							&& !SSE_QUERY_TOKEN_FLAGS_HAVE_DIGIT(&tokenizer)) {
					sse_server_search_tokenizer_token_push(&tokenizer, s);
				}
				else {
					sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
				}
			}
			else if (*s == '$') {
				if (SSE_QUERY_TOKEN_IS_EMPTY(&tokenizer)) {
					sse_server_search_tokenizer_token_push(&tokenizer, s);
				}
				else {
					sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
				}
			}
			else if (*s == '%') {
				if (!SSE_QUERY_TOKEN_FLAGS_HAVE_ALPHA(&tokenizer)
							&& SSE_QUERY_TOKEN_FLAGS_HAVE_DIGIT(&tokenizer)) {
					sse_server_search_tokenizer_token_push(&tokenizer, s);
				}
				sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
			}
			else if (*s == ',' || *s == '.') {	
				if (!SSE_QUERY_TOKEN_FLAGS_HAVE_ALPHA(&tokenizer)
							&& SSE_QUERY_TOKEN_FLAGS_HAVE_DIGIT(&tokenizer)) {
					sse_server_search_tokenizer_token_push(&tokenizer, s);
					if (*s == ',') {
						SSE_QUERY_TOKEN_FLAGS_SET_COMMA(&tokenizer);
					}
					else {
						SSE_QUERY_TOKEN_FLAGS_SET_PERIOD(&tokenizer);
					}
				}
				else {
					sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
				}

			}
			else if (isspace(*s)) {
				sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
			}
			
			else {
				sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
			}
		}
		else {
			if (isspace(*s)) {
				sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
			}
		}
		++s;
	} /* end while */
	
	sse_server_search_tokenizer_token_fetch(&tokenizer, query, server);
}

static void
sse_server_search_tokenizer_token_push(sse_server_search_query_tokenizer_t *tokenizer, char *address)
{
	if (SSE_QUERY_TOKEN_IS_EMPTY(tokenizer)) {
		tokenizer->token.data = address;
	}
	++tokenizer->token.len;
}

static void
sse_server_search_tokenizer_token_fetch(sse_server_search_query_tokenizer_t *tokenizer, sse_server_search_query_t *query, sse_server_t *server)
{
	char								tail;
	sse_server_search_query_term_t		query_term;

	if (!SSE_QUERY_TOKEN_IS_EMPTY(tokenizer)) {
		if ((tail = tokenizer->token.data[tokenizer->token.len - 1]) == ',' || tail == '.' || tail == '-' || tail == '$') {
			--tokenizer->token.len;
		}
		if (SSE_QUERY_TOKEN_IS_EMPTY(tokenizer)) {
			SSE_QUERY_TOKEN_EMPTY(tokenizer);
			return;
		}

		/* lower the token */
		sse_server_search_tokenizer_token_tolower(&tokenizer->token);
		/* filter stop words */
		if (!sse_hash_contains(server->stopwords_dict, &tokenizer->token)) {
			/* stemming */
			tokenizer->token.len = stem(tokenizer->token.data, 0, tokenizer->token.len - 1) + 1;
			/* add into term vector */
			if (!sse_hash_contains(query->term_hash, &tokenizer->token)) {
				sse_str_assign(&query_term.term, &tokenizer->token);
				sse_vector_push_back(query->posting_list, &query_term);
				sse_hash_add(query->term_hash, &((sse_server_search_query_term_t *)sse_vector_top(query->posting_list))->term, SSE_NULL);
			}
		}

		/* clean up the token */
		SSE_QUERY_TOKEN_EMPTY(tokenizer);
	}
}

static void
sse_server_search_tokenizer_token_tolower(sse_str_t *token)
{
	size_t len = token->len;
	char *c;

	c = token->data;
	while (len) {
		if (*c >= 'A' && *c <= 'Z') {
			*c = *c | 0x20;
		}
		++c;
		--len;
	}
}
