#ifndef _SSE_POOL_H_INCLUDED_
#define _SSE_POOL_H_INCLUDED_

#include <sse_core.h>

typedef struct sse_pool_small_block_s	sse_pool_small_block_t;
typedef struct sse_pool_large_block_s	sse_pool_large_block_t;

struct sse_pool_small_block_s {
	char						*last;
	char						*end;
	sse_pool_small_block_t		*next;
};

struct sse_pool_large_block_s {
	sse_pool_large_block_t		*next;
	void						*data;
};

typedef struct {
	sse_pool_small_block_t		*small;
	size_t						small_block_size;
	sse_uint_t					small_block_count;
	sse_pool_small_block_t		*current;
	sse_pool_large_block_t		*large;
	sse_log_t					*log;
} sse_pool_t;


sse_pool_t *sse_pool_create(size_t size, sse_log_t *log);
void sse_pool_destory(sse_pool_t *pool);
void *sse_pool_alloc(sse_pool_t *pool, size_t size);
void *sse_pool_calloc(sse_pool_t *pool, size_t size);
void sse_pool_debug_info(sse_pool_t *pool);

#endif /* _SSE_POOL_H_INCLUDED_ */
