#ifndef _SSE_TOKENIZER_H_INCLUDED_
#define _SSE_TOKENIZER_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

#define SSE_TOKENIZER_PATH_SIZE						1024
#define SSE_TOKENIZER_TOKEN_BUF_SIZE				256
#define SSE_TOKENIZER_STOPWORDS_HASHTABLE_SIZE		4096
#define SSE_TOKENIZER_STOPWORD_MAX_SIZE				128

#define SSE_TOKEN_BUF_EMPTY(buf)				(buf)->pos = 0; (buf)->flags = 0; (buf)->offset = 0
#define SSE_TOKEN_BUF_IS_FULL(buf)				((buf)->pos == (buf)->size)
#define SSE_TOKEN_BUF_IS_EMPTY(buf)				((buf)->pos == 0)
#define SSE_TOKEN_BUF_FLAGS_SET_ALPHA(buf)		(buf)->flags = (buf)->flags | 0x01
#define SSE_TOKEN_BUF_FLAGS_SET_DIGIT(buf)		(buf)->flags = (buf)->flags | 0x02
#define SSE_TOKEN_BUF_FLAGS_SET_COMMA(buf)		(buf)->flags = (buf)->flags | 0x04
#define SSE_TOKEN_BUF_FLAGS_SET_PERIOD(buf)		(buf)->flags = (buf)->flags | 0x08
#define SSE_TOKEN_BUF_FLAGS_HAVE_ALPHA(buf)		((buf)->flags & 0x01)
#define SSE_TOKEN_BUF_FLAGS_HAVE_DIGIT(buf)		((buf)->flags & 0x02)
#define SSE_TOKEN_BUF_FLAGS_HAVE_COMMA(buf)		((buf)->flags & 0x04)
#define SSE_TOKEN_BUF_FLAGS_HAVE_PERIOD(buf)	((buf)->flags & 0x08)

typedef struct  {
	char				*buf;
	sse_uint_t			pos;
	size_t				size;
	unsigned char		flags;
	sse_off_t			offset; /* store the offset of the first char in the buffer */
} sse_token_buf_t;

typedef struct {
	sse_hash_t						*stopwords_dict;

	sse_token_buf_t					*token_buf;
	sse_str_t						*token;

	sse_id_t						cur_doc_id;
	sse_off_t						cur_offset;

	sse_index_build_counter_t		*counter;

	sse_pool_t						*pool;
	sse_log_t						*log;
} sse_tokenizer_t;


sse_tokenizer_t *sse_tokenizer_create(sse_index_build_counter_t *counter, sse_pool_t *pool, sse_log_t *log);
int sse_tokenizer_execute(sse_tokenizer_t *tokenizer);

#endif /* _SSE_TOKENIZER_H_INCLUDED_ */
