#ifndef _SSE_INDEX_H_INCLUDED_
#define _SSE_INDEX_H_INCLUDED_

#include <sse_core.h>

typedef sse_uint_t		sse_id_t;
typedef sse_uint_t		sse_freq_t;
typedef off_t			sse_off_t;

typedef struct {
	FILE	*term_dict_file;
	FILE	*docid_freq_file;
	FILE	*offset_file;

	char	*term_dict_file_path;
	char	*docid_freq_file_path;
	char	*offset_file_path;
} sse_index_file_set_t;

typedef struct {
	sse_id_t		doc_id;
	sse_freq_t		freq;
} sse_term_docid_freq_t;

typedef struct {
	sse_off_t		start;
	size_t			len;
} sse_term_offset_t;

typedef struct {
	sse_str_t		*term;
	sse_freq_t		doc_freq;
	sse_vector_t	*ids_freqs;
	sse_vector_t	*offsets;
	
} sse_term_dict_index_data_t;

typedef struct {
	sse_uint_t				num;
	sse_uint_t				term_count;
	sse_uint_t				term_block_size;
	sse_str_t				prev_term;
	sse_str_t				cur_term;
	sse_freq_t				doc_freq;
	size_t					prev_docid_freq_offset;
	sse_index_file_set_t	*index_files;
} sse_index_merge_status_t;

typedef struct {
	sse_uint_t						index_file_count;
	sse_uint_t						term_pos;
	sse_off_t						prev_docid_freq_offset;
	sse_term_dict_index_data_t		*prev_index_data;
} sse_index_build_counter_t;


#include <sse_indexer_config.h>
#include <sse_tokenizer.h>
#include <sse_stemmer.h>
#include <sse_term_dict.h>
#include <sse_index_file.h>
#include <sse_index_merge.h>

#endif /* _SSE_INDEX_H_INCLUDED_ */
