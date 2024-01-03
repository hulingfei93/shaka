#include <sse_core.h>
#include <sse_index.h>

static void sse_index_merge_file_set_init(sse_index_file_set_t *index_files, const char *dir_path, const char *name);
static void sse_index_merge_file_set_open(sse_index_file_set_t *index_files, const char *type);
static void sse_index_merge_file_set_close(sse_index_file_set_t *index_files);
static void sse_index_merge_imp(sse_index_file_set_t *index_files_1, sse_index_file_set_t *index_files_2, sse_index_file_set_t *index_files_merge, sse_index_merger_t *merger);
static void sse_index_merge_term_merge(sse_index_merge_status_t *merge_to, sse_index_merge_status_t *merge_from1, sse_index_merge_status_t *merge_from2);
static void sse_index_merge_term_write(sse_index_merge_status_t	*merge_to, sse_index_merge_status_t *merge_from);
static void sse_index_merge_term_read(sse_index_merge_status_t *merge);
static void sse_index_term_dict_index_generate(const char *term_dict_path, sse_index_merger_t *merger);
static void sse_index_term_weight_generate(sse_index_merger_t *merger);

sse_index_merger_t *
sse_index_merger_create(sse_uint_t index_files_base, sse_uint_t index_files_num, sse_pool_t *pool, sse_log_t *log)
{
	sse_index_merger_t *merger;

	merger = (sse_index_merger_t *)sse_pool_alloc(pool, sizeof(sse_index_merger_t));
	if (!merger) {
		return SSE_NULL;
	}

	merger->index_files_base = index_files_base;
	merger->index_files_num = index_files_num;
	
	merger->pool = pool;
	merger->log = log;

	return merger;
}

void
sse_index_merge(sse_index_merger_t *merger)
{
	sse_uint_t	start_num, end_num, cur_num, stop_num;
	sse_index_file_set_t index_files_1, index_files_2, index_files_merge;
	char	*count_buf, *temp_dir_path, *dir_path;
	size_t	temp_dir_path_size, dir_path_size;

	if (merger->index_files_num < 1) {
		return;
	}

	index_files_1.term_dict_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_1.docid_freq_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_1.offset_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_2.term_dict_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_2.docid_freq_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_2.offset_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_merge.term_dict_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_merge.docid_freq_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	index_files_merge.offset_file_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	count_buf = (char *)sse_pool_alloc(merger->pool, SSE_INT_MAX_SIZE);
	temp_dir_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE);
	dir_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE);

	sse_strcpy(temp_dir_path, CONFIG.TEMP_DIR);
	temp_dir_path_size = sse_strlen(CONFIG.TEMP_DIR);
	if (temp_dir_path[temp_dir_path_size - 1] != '/') {
		temp_dir_path[temp_dir_path_size++] = '/';
	}

	sse_strcpy(dir_path, CONFIG.INDEX_DIR);
	dir_path_size = sse_strlen(CONFIG.INDEX_DIR);
	if (dir_path[dir_path_size - 1] != '/') {
		dir_path[dir_path_size++] = '/';
	}

	start_num = merger->index_files_base;
	end_num = merger->index_files_base + merger->index_files_num - 1;
	cur_num = start_num;

	while (cur_num < end_num) {
		stop_num = end_num;
		while (cur_num <= stop_num) {
			if (cur_num < stop_num) {
				/* create the first src merge file set */
				snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", cur_num);
				sse_index_merge_file_set_init(&index_files_1, temp_dir_path, count_buf);
				sse_index_merge_file_set_open(&index_files_1, "r");
				++cur_num;

				/* create the second src merge file set */
				snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", cur_num);
				sse_index_merge_file_set_init(&index_files_2, temp_dir_path, count_buf);
				sse_index_merge_file_set_open(&index_files_2, "r");
				++cur_num;

				/* create the destination merge file set */
				++end_num;
				snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", end_num);
				sse_index_merge_file_set_init(&index_files_merge, temp_dir_path, count_buf);
				sse_index_merge_file_set_open(&index_files_merge, "w");
				
				/* merge */
				sse_index_merge_imp(&index_files_1, &index_files_2, &index_files_merge, merger);

				/* close files */
				sse_index_merge_file_set_close(&index_files_1);
				sse_index_merge_file_set_close(&index_files_2);
				sse_index_merge_file_set_close(&index_files_merge);
			}
			else {
				/* old */
				snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", cur_num);
				sse_index_merge_file_set_init(&index_files_1, temp_dir_path, count_buf);
				++cur_num;

				/* new */
				++end_num;
				snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", end_num);
				sse_index_merge_file_set_init(&index_files_2, temp_dir_path, count_buf);

				/* rename */
				sse_file_rename(index_files_1.term_dict_file_path, index_files_2.term_dict_file_path);
				sse_file_rename(index_files_1.docid_freq_file_path, index_files_2.docid_freq_file_path);
				sse_file_rename(index_files_1.offset_file_path, index_files_2.offset_file_path);
			}
		} /* end while */
		
	} /* end while */

	/* copy the final index files to index directory */
	snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", end_num);
	sse_index_merge_file_set_init(&index_files_1, temp_dir_path, count_buf);
	sse_index_merge_file_set_init(&index_files_2, dir_path, CONFIG.INDEX_NAME);

	sse_file_copy(index_files_2.term_dict_file_path, index_files_1.term_dict_file_path);
	sse_file_copy(index_files_2.docid_freq_file_path, index_files_1.docid_freq_file_path);
	sse_file_copy(index_files_2.offset_file_path, index_files_1.offset_file_path);

	/* generate the term dictionary index file */
	sse_index_term_dict_index_generate(index_files_2.term_dict_file_path, merger);

	/* generate weight index file */
	sse_index_term_weight_generate(merger);
}

static void
sse_index_term_dict_index_generate(const char *term_dict_path, sse_index_merger_t *merger)
{
	char			*term_dict_index_path;
	size_t			 term_len, prefix_len, suffix_len;
	FILE			*term_dict_file, *term_dict_index_file;
	sse_uint_t		term_count, term_block_size, term_block_count, i;
	sse_off_t		cur_term_dict_offset;
	char			term_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];

	term_dict_index_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE); 
	sse_file_path_fill(term_dict_index_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_TERM_DICT_INDEX_FILE_EXTENSION);

	term_dict_file = fopen(term_dict_path, "r");
	term_dict_index_file = fopen(term_dict_index_path, "w");

	/* term count */
	term_count = sse_stdio_read_uint(term_dict_file);
	term_block_count = 0;
	sse_stdio_write_uint(term_block_count, term_dict_index_file);
	
	/* term block size */
	term_block_size = sse_stdio_read_vuint(term_dict_file);

	/* read the next term */
	for (i = 0; i < term_count; ++i) {
		/* read head term in the block */
		if (i % term_block_size == 0) {
			
			/* write the offset to term dictionary index file */
			cur_term_dict_offset = ftello(term_dict_file);
			sse_stdio_write_uint(cur_term_dict_offset, term_dict_index_file);
			++term_block_count;
			
			/* [string length][string]  */
			term_len = sse_stdio_read_vuint(term_dict_file);
			sse_stdio_read(term_buf, term_len, term_dict_file);
		}
		/* read no head term in the block */
		else {
			/* [prefix length][suffix length] */
			prefix_len = sse_stdio_read_vuint(term_dict_file);
			suffix_len = sse_stdio_read_vuint(term_dict_file);
			/* [suffix] */
			sse_stdio_read(term_buf, suffix_len, term_dict_file);
		}
		/* [doc frequency]  */
		sse_stdio_read_vuint(term_dict_file);
		/* [offset] in docid-freq index file */
		sse_stdio_read_vuint(term_dict_file);
	}

	/* correct the term block count */
	fseeko(term_dict_index_file, 0, SEEK_SET);
	sse_stdio_write_uint(term_block_count, term_dict_index_file);
	
	fclose(term_dict_file);
	fclose(term_dict_index_file);
}

static void
sse_index_term_weight_generate(sse_index_merger_t *merger)
{
	char	*term_dict_path, *docid_freq_path, *doc_data_index_path;
	FILE	*term_dict_file, *docid_freq_file, *doc_data_index_file;
	sse_uint_t		term_count, term_block_size, i, j;
	size_t			term_len, prefix_len, suffix_len;
	char			term_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	sse_uint_t		doc_count;
	sse_freq_t		doc_freq, term_freq;
	sse_id_t		doc_id, prev_doc_id;
	float			weight, normalized_weight;
	float			*doc_len_array;
	sse_off_t		weight_offset = -1 * sizeof(float);

	term_dict_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE);
	docid_freq_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE);
	doc_data_index_path = (char *)sse_pool_alloc(merger->pool, SSE_PATH_MAX_SIZE);

	sse_file_path_fill(term_dict_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_TERM_DICT_FILE_EXTENSION);
	sse_file_path_fill(docid_freq_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION);
	sse_file_path_fill(doc_data_index_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);

	term_dict_file = fopen(term_dict_path, "r");
	docid_freq_file = fopen(docid_freq_path, "r+");
	
	doc_data_index_file = fopen(doc_data_index_path, "r");
	doc_count = sse_stdio_read_uint(doc_data_index_file);
	fclose(doc_data_index_file);

	doc_len_array = (float *)sse_pool_calloc(merger->pool, doc_count * sizeof(float));

	/*********compute and save weight for each term******************/
	/* term count */
	term_count = sse_stdio_read_uint(term_dict_file);
	
	/* term block size */
	term_block_size = sse_stdio_read_vuint(term_dict_file);

	/* read head term in the block */
	for (i = 0; i < term_count; ++i) {
		/* [string length][string] */
		if (i % term_block_size == 0) {
			term_len = sse_stdio_read_vuint(term_dict_file);
			sse_stdio_read(term_buf, term_len, term_dict_file);
		}
		/* [prefix length][suffix length][suffix] */
		else {
			prefix_len = sse_stdio_read_vuint(term_dict_file);
			suffix_len = sse_stdio_read_vuint(term_dict_file);
			sse_stdio_read(term_buf, suffix_len, term_dict_file);
		}
		/* [doc frequency] */
		doc_freq = sse_stdio_read_vuint(term_dict_file);
		/* [offset] in docid-freq index file */
		sse_stdio_read_vuint(term_dict_file);

		for (j = 0; j < doc_freq; ++j) {
			/* [doc id](d-gap) */
			if (j == 0) {
				doc_id = sse_stdio_read_vuint(docid_freq_file);
			}
			else {
				doc_id = prev_doc_id + sse_stdio_read_vuint(docid_freq_file);
			}
			prev_doc_id = doc_id;

			/* [term frequency] */
			term_freq = sse_stdio_read_vuint(docid_freq_file);

			/* write the [weight] */
			weight = term_freq * log10f(doc_count / (float)doc_freq);
			sse_stdio_write_float(weight, docid_freq_file);
			doc_len_array[doc_id - 1] += weight * weight;

			/* [offset](d-gap) in offset index file */
			sse_stdio_read_vuint(docid_freq_file);
		}
	}

	/**********compute length for each doc****************/
	for (i = 0; i< doc_count; ++i) {
		doc_len_array[i] = sqrtf(doc_len_array[i]);
	}
	
	/*********normalize weight******************/
	fseeko(term_dict_file, 0, SEEK_SET);
	fseeko(docid_freq_file, 0, SEEK_SET);

	/* term count */
	term_count = sse_stdio_read_uint(term_dict_file);
	
	/* term block size */
	term_block_size = sse_stdio_read_vuint(term_dict_file);

	/* read head term in the block */
	for (i = 0; i < term_count; ++i) {
		/* [string length][string] */
		if (i % term_block_size == 0) {
			term_len = sse_stdio_read_vuint(term_dict_file);
			sse_stdio_read(term_buf, term_len, term_dict_file);
		}
		/* [prefix length][suffix length][suffix] */
		else {
			prefix_len = sse_stdio_read_vuint(term_dict_file);
			suffix_len = sse_stdio_read_vuint(term_dict_file);
			sse_stdio_read(term_buf, suffix_len, term_dict_file);
		}
		/* [doc frequency] */
		doc_freq = sse_stdio_read_vuint(term_dict_file);
		/* [offset] in docid-freq index file */
		sse_stdio_read_vuint(term_dict_file);

		for (j = 0; j < doc_freq; ++j) {
			/* [doc id](d-gap) */
			if (j == 0) {
				doc_id = sse_stdio_read_vuint(docid_freq_file);
			}
			else {
				doc_id = prev_doc_id + sse_stdio_read_vuint(docid_freq_file);
			}
			prev_doc_id = doc_id;

			/* [term frequency] */
			term_freq = sse_stdio_read_vuint(docid_freq_file);

			/* write the [weight] */
			weight = sse_stdio_read_float(docid_freq_file);
			if (doc_len_array[doc_id - 1] != 0.0f) { 
				normalized_weight = weight / doc_len_array[doc_id - 1];
				fseeko(docid_freq_file, weight_offset, SEEK_CUR);
				sse_stdio_write_float(normalized_weight, docid_freq_file);
			}

			/* [offset](d-gap) in offset index file */
			sse_stdio_read_vuint(docid_freq_file);
		}
	}
	
	fclose(term_dict_file);
	fclose(docid_freq_file);
}


static void
sse_index_merge_file_set_init(sse_index_file_set_t *index_files, const char *dir_path, const char *name)
{
	size_t dir_path_size, name_size;

	dir_path_size = sse_strlen(dir_path);
	name_size = sse_strlen(name);

	sse_strcpy(index_files->term_dict_file_path, dir_path);
	sse_strcpy(index_files->term_dict_file_path + dir_path_size, name);
	sse_strcpy(index_files->term_dict_file_path + dir_path_size + name_size, CONFIG.INDEX_TERM_DICT_FILE_EXTENSION);
	sse_strcpy(index_files->docid_freq_file_path, dir_path);
	sse_strcpy(index_files->docid_freq_file_path + dir_path_size, name);
	sse_strcpy(index_files->docid_freq_file_path + dir_path_size + name_size, CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION);	
	sse_strcpy(index_files->offset_file_path, dir_path);
	sse_strcpy(index_files->offset_file_path + dir_path_size, name);
	sse_strcpy(index_files->offset_file_path + dir_path_size + name_size, CONFIG.INDEX_OFFSET_FILE_EXTENSION);
}

static void
sse_index_merge_file_set_open(sse_index_file_set_t *index_files, const char *type)
{
	index_files->term_dict_file = fopen(index_files->term_dict_file_path, type);
	index_files->docid_freq_file = fopen(index_files->docid_freq_file_path, type);
	index_files->offset_file = fopen(index_files->offset_file_path, type);
}

static void
sse_index_merge_file_set_close(sse_index_file_set_t *index_files)
{
	fclose(index_files->term_dict_file);
	fclose(index_files->docid_freq_file);
	fclose(index_files->offset_file);
}

static void
sse_index_merge_imp(sse_index_file_set_t *index_files_1, sse_index_file_set_t *index_files_2, sse_index_file_set_t *index_files_merge, sse_index_merger_t *merger)
{
	char							prev_term_1_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
    char							cur_term_1_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	char							prev_term_2_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	char							cur_term_2_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	char							prev_term_merge_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	char							cur_term_merge_buf[SSE_TOKENIZER_TOKEN_BUF_SIZE];
	sse_index_merge_status_t		merge1, merge2, merge;
	sse_int_t						cmp_result;

	merge1.index_files = index_files_1; 
	merge2.index_files = index_files_2; 
	merge.index_files = index_files_merge;

	sse_str_set_null(&merge1.prev_term);		merge1.prev_term.data = prev_term_1_buf;
	sse_str_set_null(&merge1.cur_term);			merge1.cur_term.data = cur_term_1_buf;
	sse_str_set_null(&merge2.prev_term);		merge2.prev_term.data = prev_term_2_buf;
	sse_str_set_null(&merge2.cur_term);			merge2.cur_term.data = cur_term_2_buf;
	sse_str_set_null(&merge.prev_term);			merge.prev_term.data = prev_term_merge_buf;
	sse_str_set_null(&merge.cur_term);			merge.cur_term.data = cur_term_merge_buf;
	

	/********************merge term dictionary index files**************************/
	/* merge term count */
	merge1.term_count = sse_stdio_read_uint(merge1.index_files->term_dict_file);
	merge2.term_count = sse_stdio_read_uint(merge2.index_files->term_dict_file);
	/* write the merge term count to zero first, then correct it after merge */
	merge.term_count = 0;
	sse_stdio_write_uint(0, merge.index_files->term_dict_file);

	/* merge term block size */
	merge1.term_block_size = sse_stdio_read_vuint(merge1.index_files->term_dict_file);
	merge2.term_block_size = sse_stdio_read_vuint(merge2.index_files->term_dict_file);
	if (merge1.term_block_size != merge2.term_block_size) { 
		sse_log(SSE_LOG_FATAL, merger->log, SSE_BOOL_FALSE, "Different block size for 2 term dictionary index: %s(%u), %s(%u)", merge1.index_files->term_dict_file_path, merge1.term_block_size, merge2.index_files->term_dict_file_path, merge2.term_block_size);
		exit(1);
	}
	merge.term_block_size = merge1.term_block_size;
	sse_stdio_write_vuint(merge.term_block_size, merge.index_files->term_dict_file);

	/* merge terms */
	merge1.num = 0;
	sse_index_merge_term_read(&merge1);
	merge2.num = 0;
	sse_index_merge_term_read(&merge2);
	merge.num = 0;

	while (merge1.num < merge1.term_count && merge2.num < merge2.term_count) {
		cmp_result = sse_str_strcmp(&merge1.cur_term, &merge2.cur_term);
		if (cmp_result < 0) {
			/* merge1 -> merge */
			sse_index_merge_term_write(&merge, &merge1);
			/* read next term */
			if (++merge1.num < merge1.term_count) {
				sse_index_merge_term_read(&merge1);
			}
		}
		else if (cmp_result > 0) {
			/* merge2 -> merge */
			sse_index_merge_term_write(&merge, &merge2);
			/* read next term */
			if (++merge2.num < merge2.term_count) {
				sse_index_merge_term_read(&merge2);
			}
		}
		else {
			/* merge1 + merge2 -> merge */
			sse_index_merge_term_merge(&merge, &merge1, &merge2);
			/* read next term */
			if (++merge1.num < merge1.term_count) {
				sse_index_merge_term_read(&merge1);
			}
			if (++merge2.num < merge2.term_count) {
				sse_index_merge_term_read(&merge2);
			}
		}	
	}

	while (merge1.num < merge1.term_count) {
		sse_index_merge_term_write(&merge, &merge1);
		if (++merge1.num < merge1.term_count) {
			sse_index_merge_term_read(&merge1);
		}
	}
	
	while (merge2.num < merge2.term_count) {
		sse_index_merge_term_write(&merge, &merge2);
		if (++merge2.num < merge2.term_count) {
			sse_index_merge_term_read(&merge2);
		}
	}

	/* correct the merge term count */
	fseeko(merge.index_files->term_dict_file, 0, SEEK_SET);
	sse_stdio_write_uint(merge.term_count, merge.index_files->term_dict_file);

}

static void
sse_index_merge_term_merge(sse_index_merge_status_t *merge_to, sse_index_merge_status_t *merge_from1, sse_index_merge_status_t *merge_from2)
{
	sse_str_t						*cur_term, *prev_term;
	size_t							prefix_len, diff_len, len;
	sse_off_t						cur_docid_freq_offset, cur_offset_offset, prev_offset_offset, start;
	sse_uint_t						i, j;
	sse_id_t						doc_id, doc_id_diff, doc_id_diff_1, doc_id_diff_2, doc_id_first, doc_id_second;
	sse_freq_t						freq, freq_1, freq_2, freq_first, freq_second;
	sse_index_merge_status_t		*merge_first, *merge_second;

	prev_term = &merge_to->prev_term;
	cur_term = &merge_from1->cur_term;

	++merge_to->term_count;
	/*******************merge term dictionary index file*******************/
	/* write head term in the block*/
	if (merge_to->num % merge_to->term_block_size == 0) {
		/* [string length][string] */
		sse_stdio_write_vuint(cur_term->len, merge_to->index_files->term_dict_file);
		sse_stdio_write(cur_term->data, merge_from1->cur_term.len, merge_to->index_files->term_dict_file);
		/* [doc frequency] */
		sse_stdio_write_vuint(merge_from1->doc_freq + merge_from2->doc_freq, merge_to->index_files->term_dict_file);
		/* [offset](d-gap) in docid-freq index file */
		cur_docid_freq_offset = ftello(merge_to->index_files->docid_freq_file);
		sse_stdio_write_vuint(cur_docid_freq_offset, merge_to->index_files->term_dict_file);
	}
	/* write no head term in the block */
	else {
		/* [prefix length][suffix length][suffix] */
		prefix_len = sse_term_compute_prefix(prev_term, cur_term);
		diff_len = cur_term->len - prefix_len;
		sse_stdio_write_vuint(prefix_len, merge_to->index_files->term_dict_file);
		sse_stdio_write_vuint(diff_len, merge_to->index_files->term_dict_file);
		sse_stdio_write(cur_term->data + prefix_len, diff_len, merge_to->index_files->term_dict_file);
		/* [doc frequency] */
		sse_stdio_write_vuint(merge_from1->doc_freq + merge_from2->doc_freq, merge_to->index_files->term_dict_file);
		/* [offset](d-gap) in docid-freq index file */
		cur_docid_freq_offset = ftello(merge_to->index_files->docid_freq_file);
		sse_stdio_write_vuint(cur_docid_freq_offset - merge_to->prev_docid_freq_offset, merge_to->index_files->term_dict_file);
	}
	merge_to->prev_docid_freq_offset = cur_docid_freq_offset;

	/*******************merge docid-freq index file*************************/
	doc_id_diff_1 = sse_stdio_read_vuint(merge_from1->index_files->docid_freq_file);
	freq_1 = sse_stdio_read_vuint(merge_from1->index_files->docid_freq_file);
	sse_stdio_read_float(merge_from1->index_files->docid_freq_file);
	sse_stdio_read_vuint(merge_from1->index_files->docid_freq_file);

	doc_id_diff_2 = sse_stdio_read_vuint(merge_from2->index_files->docid_freq_file);
	freq_2 = sse_stdio_read_vuint(merge_from2->index_files->docid_freq_file);
	sse_stdio_read_float(merge_from2->index_files->docid_freq_file);
	sse_stdio_read_vuint(merge_from2->index_files->docid_freq_file);

	if(doc_id_diff_1 < doc_id_diff_2) {
		merge_first = merge_from1;
		merge_second = merge_from2;
		doc_id_first = doc_id_diff_1;
		doc_id_second = doc_id_diff_2;
		freq_first = freq_1;
		freq_second = freq_2;
	}
	else {
		merge_first = merge_from2;
		merge_second = merge_from1;
		doc_id_first = doc_id_diff_2;
		doc_id_second = doc_id_diff_1;
		freq_first = freq_2;
		freq_second = freq_1;
	}

	/* write the first [docid-freq-offset] in first merge */
	sse_stdio_write_vuint(doc_id_first, merge_to->index_files->docid_freq_file);
	doc_id = doc_id_first;
	sse_stdio_write_vuint(freq_first, merge_to->index_files->docid_freq_file);
	sse_stdio_write_float(0.0f, merge_to->index_files->docid_freq_file);
	cur_offset_offset = ftello(merge_to->index_files->offset_file);
	sse_stdio_write_vuint(cur_offset_offset, merge_to->index_files->docid_freq_file);
	prev_offset_offset = cur_offset_offset;
	for (j = 0; j < freq_first; ++j) {
		start = sse_stdio_read_vuint(merge_first->index_files->offset_file);
		len = sse_stdio_read_vuint(merge_first->index_files->offset_file);
		sse_stdio_write_vuint(start, merge_to->index_files->offset_file);
		sse_stdio_write_vuint(len, merge_to->index_files->offset_file);
	}

	/* write the left [docid-freq-offset] in first merge */
	for (i = 1; i < merge_first->doc_freq; ++i) {
		/* [doc id](d-gap) */
		doc_id_diff = sse_stdio_read_vuint(merge_first->index_files->docid_freq_file);
		sse_stdio_write_vuint(doc_id_diff, merge_to->index_files->docid_freq_file);
		/* save the actual doc id */
		doc_id += doc_id_diff;
		
		/* [term frequency] */
		freq = sse_stdio_read_vuint(merge_first->index_files->docid_freq_file);
		sse_stdio_write_vuint(freq, merge_to->index_files->docid_freq_file);
		
		/* [weight] */
		sse_stdio_read_float(merge_first->index_files->docid_freq_file);
		sse_stdio_write_float(0.0f, merge_to->index_files->docid_freq_file);
		
		/* [offset](d-gap) in offset index file */
		sse_stdio_read_vuint(merge_first->index_files->docid_freq_file);
		cur_offset_offset = ftello(merge_to->index_files->offset_file);
		sse_stdio_write_vuint(cur_offset_offset - prev_offset_offset, merge_to->index_files->docid_freq_file);
		prev_offset_offset = cur_offset_offset;

		/***************merge offset index file****************/
		for (j = 0; j < freq; ++j) {
			start = sse_stdio_read_vuint(merge_first->index_files->offset_file);
			len = sse_stdio_read_vuint(merge_first->index_files->offset_file);
			sse_stdio_write_vuint(start, merge_to->index_files->offset_file);
			sse_stdio_write_vuint(len, merge_to->index_files->offset_file);
		}
	}
	
	/* write the first [docid-freq-offset] in second merge */
	sse_stdio_write_vuint(doc_id_second - doc_id, merge_to->index_files->docid_freq_file);
	sse_stdio_write_vuint(freq_second, merge_to->index_files->docid_freq_file);
	sse_stdio_write_float(0.0f, merge_to->index_files->docid_freq_file);
	cur_offset_offset = ftello(merge_to->index_files->offset_file);
	sse_stdio_write_vuint(cur_offset_offset - prev_offset_offset, merge_to->index_files->docid_freq_file);
	prev_offset_offset = cur_offset_offset;
	for (j = 0; j < freq_second; ++j) {
		start = sse_stdio_read_vuint(merge_second->index_files->offset_file);
		len = sse_stdio_read_vuint(merge_second->index_files->offset_file);
		sse_stdio_write_vuint(start, merge_to->index_files->offset_file);
		sse_stdio_write_vuint(len, merge_to->index_files->offset_file);
	}
	
	/* write the left [docid-freq-offset] in second  merge */
	for (i = 1; i < merge_second->doc_freq; ++i) {
		/* [doc id](d-gap) */
		doc_id_diff = sse_stdio_read_vuint(merge_second->index_files->docid_freq_file);
		sse_stdio_write_vuint(doc_id_diff, merge_to->index_files->docid_freq_file);
		
		/* [term frequency] */
		freq = sse_stdio_read_vuint(merge_second->index_files->docid_freq_file);
		sse_stdio_write_vuint(freq, merge_to->index_files->docid_freq_file);
		
		/* [weight] */
		sse_stdio_read_float(merge_second->index_files->docid_freq_file);
		sse_stdio_write_float(0.0f, merge_to->index_files->docid_freq_file);
		
		/* [offset](d-gap) in offset index file */
		sse_stdio_read_vuint(merge_second->index_files->docid_freq_file);
		cur_offset_offset = ftello(merge_to->index_files->offset_file);
		sse_stdio_write_vuint(cur_offset_offset - prev_offset_offset, merge_to->index_files->docid_freq_file);
		prev_offset_offset = cur_offset_offset;

		/***************merge offset index file****************/
		for (j = 0; j < freq; ++j) {
			start = sse_stdio_read_vuint(merge_second->index_files->offset_file);
			len = sse_stdio_read_vuint(merge_second->index_files->offset_file);
			sse_stdio_write_vuint(start, merge_to->index_files->offset_file);
			sse_stdio_write_vuint(len, merge_to->index_files->offset_file);
		}
	}

	/* save prev term */
	sse_str_copy(&merge_to->prev_term, &merge_from1->cur_term);
	
	++merge_to->num;
}

static void
sse_index_merge_term_write(sse_index_merge_status_t	*merge_to, sse_index_merge_status_t *merge_from)
{
	sse_str_t	*cur_term, *prev_term;
	size_t		prefix_len, diff_len, len;
	sse_off_t	cur_docid_freq_offset, cur_offset_offset, prev_offset_offset, start;
	sse_uint_t	i, j;
	sse_id_t	doc_id_diff;
	sse_freq_t	freq;

	prev_term = &merge_to->prev_term;
	cur_term = &merge_from->cur_term;

	++merge_to->term_count;
	/***********merge term dictionary index file***************/
	/* write head term in the block */
	if (merge_to->num % merge_to->term_block_size == 0) {
		/* [string length][string] */
		sse_stdio_write_vuint(cur_term->len, merge_to->index_files->term_dict_file);
		sse_stdio_write(cur_term->data, merge_from->cur_term.len, merge_to->index_files->term_dict_file);
		/* [doc frequency] */
		sse_stdio_write_vuint(merge_from->doc_freq, merge_to->index_files->term_dict_file);
		/* [offset](d-gap) in docid-freq index file */
		cur_docid_freq_offset = ftello(merge_to->index_files->docid_freq_file);
		sse_stdio_write_vuint(cur_docid_freq_offset, merge_to->index_files->term_dict_file);
	}
	/* write no head term in the block */
	else {
		/* [prefix length][suffix length][suffix] */
		prefix_len = sse_term_compute_prefix(prev_term, cur_term);
		diff_len = cur_term->len - prefix_len;
		sse_stdio_write_vuint(prefix_len, merge_to->index_files->term_dict_file);
		sse_stdio_write_vuint(diff_len, merge_to->index_files->term_dict_file);
		sse_stdio_write(cur_term->data + prefix_len, diff_len, merge_to->index_files->term_dict_file);
		/* [doc frequency] */
		sse_stdio_write_vuint(merge_from->doc_freq, merge_to->index_files->term_dict_file);
		/* [offset](d-gap) in docid-freq index file */
		cur_docid_freq_offset = ftello(merge_to->index_files->docid_freq_file);
		sse_stdio_write_vuint(cur_docid_freq_offset - merge_to->prev_docid_freq_offset, merge_to->index_files->term_dict_file);
	}
	merge_to->prev_docid_freq_offset = cur_docid_freq_offset;

	/*******************merge docid-freq index file*************************/
	for (i = 0; i < merge_from->doc_freq; ++i) {
		/* [doc id](d-gap) */
		doc_id_diff = sse_stdio_read_vuint(merge_from->index_files->docid_freq_file);
		sse_stdio_write_vuint(doc_id_diff, merge_to->index_files->docid_freq_file);
		
		/* [term frequency] */
		freq = sse_stdio_read_vuint(merge_from->index_files->docid_freq_file);
		sse_stdio_write_vuint(freq, merge_to->index_files->docid_freq_file);
		
		/* [weight] */
		sse_stdio_read_float(merge_from->index_files->docid_freq_file);
		sse_stdio_write_float(0.0f, merge_to->index_files->docid_freq_file);
		
		/* [offset](d-gap) in offset index file */
		sse_stdio_read_vuint(merge_from->index_files->docid_freq_file);
		cur_offset_offset = ftello(merge_to->index_files->offset_file);
		if (i == 0) {
			sse_stdio_write_vuint(cur_offset_offset, merge_to->index_files->docid_freq_file);
		}
		else {
			sse_stdio_write_vuint(cur_offset_offset - prev_offset_offset, merge_to->index_files->docid_freq_file);
		}
		prev_offset_offset = cur_offset_offset;

		/***************merge offset index file****************/
		for (j = 0; j < freq; ++j) {
			start = sse_stdio_read_vuint(merge_from->index_files->offset_file);
			len = sse_stdio_read_vuint(merge_from->index_files->offset_file);
			sse_stdio_write_vuint(start, merge_to->index_files->offset_file);
			sse_stdio_write_vuint(len, merge_to->index_files->offset_file);
		}
	}
	
	/* save prev term */
	sse_str_copy(&merge_to->prev_term, &merge_from->cur_term);
	
	++merge_to->num;
}

static void
sse_index_merge_term_read(sse_index_merge_status_t *merge)
{
	/* save prev term */
	sse_str_copy(&merge->prev_term, &merge->cur_term);
	
	/* read head term in the block */
	if (merge->num % merge->term_block_size == 0) {
		/* [string length][string]  */
		merge->cur_term.len = sse_stdio_read_vuint(merge->index_files->term_dict_file);
		sse_stdio_read(merge->cur_term.data, merge->cur_term.len, merge->index_files->term_dict_file);
	}
	/* read no head term in the block */
	else {
		size_t prefix_len, suffix_len;
		/* [prefix length][suffix length] */
		prefix_len = sse_stdio_read_vuint(merge->index_files->term_dict_file);
		suffix_len = sse_stdio_read_vuint(merge->index_files->term_dict_file);
		merge->cur_term.len = prefix_len + suffix_len;
		if (prefix_len > 0) {
			sse_memcpy(merge->cur_term.data, merge->prev_term.data, prefix_len);
		}
		/* [suffix] */
		sse_stdio_read(merge->cur_term.data + prefix_len, suffix_len, merge->index_files->term_dict_file);
	}
	/* [doc frequency]  */
  	merge->doc_freq = sse_stdio_read_vuint(merge->index_files->term_dict_file);
	/* [offset] in docid-freq index file */
	sse_stdio_read_vuint(merge->index_files->term_dict_file);
}
