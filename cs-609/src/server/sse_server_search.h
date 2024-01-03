#ifndef _SSE_SERVER_SEARCH_H_INCLUDED_
#define _SSE_SERVER_SEARCH_H_INCLUDED_

#include <sse_core.h>
#include <sse_server.h>

#define SSE_QUERY_TERM_NORMAL_SIZE				10
#define SSE_QUERY_POSTING_NORMAL_SIZE			100
#define SSE_QUERY_OFFSET_NORMAL_SIZE			10
#define SSE_QUERY_RESULT_SUMMARY_MAX_SIZE		500
#define SSE_QUERY_RESULT_SUMMARY_EXTEND_SIZE	150

#define SSE_QUERY_TOKEN_EMPTY(tokenizer)				(tokenizer)->token.data = SSE_NULL, (tokenizer)->token.len = 0, (tokenizer)->flags = 0
#define SSE_QUERY_TOKEN_IS_EMPTY(tokenizer)				((tokenizer)->token.len == 0)
#define SSE_QUERY_TOKEN_FLAGS_SET_ALPHA(tokenizer)		(tokenizer)->flags = (tokenizer)->flags | 0x01
#define SSE_QUERY_TOKEN_FLAGS_SET_DIGIT(tokenizer)		(tokenizer)->flags = (tokenizer)->flags | 0x02
#define SSE_QUERY_TOKEN_FLAGS_SET_COMMA(tokenizer)		(tokenizer)->flags = (tokenizer)->flags | 0x04
#define SSE_QUERY_TOKEN_FLAGS_SET_PERIOD(tokenizer)		(tokenizer)->flags = (tokenizer)->flags | 0x08
#define SSE_QUERY_TOKEN_FLAGS_HAVE_ALPHA(tokenizer)		((tokenizer)->flags & 0x01)
#define SSE_QUERY_TOKEN_FLAGS_HAVE_DIGIT(tokenizer)		((tokenizer)->flags & 0x02)
#define SSE_QUERY_TOKEN_FLAGS_HAVE_COMMA(tokenizer)		((tokenizer)->flags & 0x04)
#define SSE_QUERY_TOKEN_FLAGS_HAVE_PERIOD(tokenizer)	((tokenizer)->flags & 0x08)

typedef enum {
	STANDARD_MODEL,
	OMIT_DOCLEN_MODEL
} sse_server_search_model_t;

typedef struct {
	sse_uint_t	term_index;
	sse_off_t	offset;			
} sse_server_search_position_t;

typedef struct {
	sse_str_t		token;
	unsigned char	flags;
} sse_server_search_query_tokenizer_t;

typedef struct {
	sse_str_t					term;
	sse_freq_t					doc_freq;
	sse_vector_t				*posting;
} sse_server_search_query_term_t;

typedef struct {
	sse_id_t		doc_id;
	float			score;
	char		    *title;
	char			*summary;
	sse_uint_t		doc_size;
} sse_server_search_result_t;

typedef struct {
	sse_hash_t						*term_hash;
	sse_vector_t					*posting_list;
	sse_heap_t						*top_docs;
	sse_server_search_result_t		*results;
	sse_uint_t						results_len;
	sse_server_search_model_t		search_model;

	sse_pool_t		*pool;
	sse_log_t		*log;
} sse_server_search_query_t;

sse_server_search_query_t * sse_server_search (char *query_text, sse_uint_t top, sse_server_search_model_t search_model, sse_server_t *server, sse_pool_t *pool, sse_log_t *log);
char * sse_server_search_doc(sse_id_t doc_id, sse_server_t *server, sse_pool_t *pool, sse_log_t *log);

#endif /* _SSE_SERVER_SEARCH_H_INCLUDED_ */
