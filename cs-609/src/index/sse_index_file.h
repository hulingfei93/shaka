#ifndef _SSE_INDEX_FILE_H_INCLUDED_
#define _SSE_INDEX_FILE_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

typedef struct {
	sse_str_t		term;
	sse_freq_t		doc_freq;
	sse_off_t		offset;

} sse_index_file_term_t;

typedef struct {
	sse_uint_t					term_num;
	sse_uint_t					term_count;
	sse_uint_t					term_block_size;

	sse_index_file_term_t		*prev_term;
	sse_index_file_term_t		*cur_term;

	FILE						*file;
	const char					*file_path;

	sse_pool_t					*pool;
	sse_log_t					*log;
	
} sse_index_file_term_dict_t;

typedef struct {
	sse_uint_t		index_count;
	sse_off_t		base_offset;
	FILE			*file;
	const char		*file_path;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_index_file_term_dict_index_t;

/* docid freq index file */
typedef struct {
	FILE			*file;
	const char		*file_path;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_index_file_docid_freq_t;

typedef struct {
	sse_id_t		doc_id;
	sse_freq_t		term_freq;
	float			weight;
	sse_off_t		offset;
} sse_index_file_posting_t;

/* offset index file */
typedef struct {
	FILE			*file;
	const char		*file_path;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_index_file_offset_t;

typedef struct {
	sse_off_t		start;
	size_t			len;
} sse_index_file_offset_data_t;

/* doc index file */
typedef struct {
	sse_uint_t		doc_count;
	sse_off_t		base_offset;
	FILE			*file;
	const char		*file_path;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_index_file_doc_index_t;

/* doc file */
typedef struct {
	FILE			*file;
	const char		*file_path;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_index_file_doc_t;

sse_index_file_term_dict_t * sse_index_file_term_dict_init(sse_pool_t *pool, sse_log_t *log);
sse_index_file_term_dict_index_t * sse_index_file_term_dict_index_init(sse_pool_t *pool, sse_log_t *log);
sse_index_file_docid_freq_t * sse_index_file_docid_freq_init(sse_pool_t *pool, sse_log_t *log);
void sse_index_file_term_dict_open(sse_index_file_term_dict_t *term_dict, const char* path, const char *type);
void sse_index_file_term_dict_index_open(sse_index_file_term_dict_index_t *term_dict_index, const char *path, const char *type);
void sse_index_file_docid_freq_open(sse_index_file_docid_freq_t *docid_freq, const char *path, const char *type);
void sse_index_file_term_dict_close(sse_index_file_term_dict_t *term_dict);
void sse_index_file_term_dict_index_close(sse_index_file_term_dict_index_t *term_dict_index);
void sse_index_file_docid_freq_close(sse_index_file_docid_freq_t *docid_freq);
sse_index_file_term_t * sse_index_file_term_dict_read(sse_index_file_term_dict_t *term_dict);
sse_index_file_term_t * sse_index_file_term_dict_random_read(sse_index_file_term_dict_t *term_dict, sse_index_file_term_dict_index_t *term_dict_index, sse_uint_t term_num);
void sse_index_file_docid_freq_read(sse_index_file_docid_freq_t *docid_freq, sse_off_t begin_offset, sse_freq_t doc_freq, sse_vector_t *posting_list);

/* doc index file */
sse_index_file_doc_index_t * sse_index_file_doc_index_init(sse_pool_t *pool, sse_log_t *log);
void sse_index_file_doc_index_open(sse_index_file_doc_index_t *doc_index, const char *path, const char *type);
void sse_index_file_doc_index_close(sse_index_file_doc_index_t *doc_index);

/* offset index file */
sse_index_file_offset_t * sse_index_file_offset_init(sse_pool_t *pool, sse_log_t *log);
void sse_index_file_offset_open(sse_index_file_offset_t *offset_file, const char *path, const char *type);
void sse_index_file_offset_close(sse_index_file_offset_t *offset_file);
void sse_index_file_offset_read(sse_index_file_offset_t *offset_file, sse_off_t begin_offset, sse_freq_t term_freq, sse_vector_t *offset_list);

/* doc file*/

sse_index_file_doc_t * sse_index_file_doc_init(sse_pool_t *pool, sse_log_t *log);
void sse_index_file_doc_open(sse_index_file_doc_t *doc, const char *path, const char *type);
void sse_index_file_doc_close(sse_index_file_doc_t *doc);
char * sse_index_file_doc_name_read(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_pool_t *pool, sse_uint_t *doc_size);
size_t sse_index_file_doc_read(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_off_t begin_offset, void *buf, size_t size);
char * sse_index_file_doc_get(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_pool_t *pool);

#endif /* _SSE_INDEX_FILE_H_INCLUDED_ */
