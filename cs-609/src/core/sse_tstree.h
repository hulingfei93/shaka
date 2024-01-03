#ifndef _SSE_TSTREE_H_INCLUDED_
#define _SSE_TSTREE_H_INCLUDED_

#include <sse_core.h>

typedef struct sse_tstree_node_s	sse_tstree_node_t;

struct sse_tstree_node_s {
	char						split_char;
	void						*data;
	sse_tstree_node_t			*left;
	sse_tstree_node_t			*right;
	sse_tstree_node_t			*middle;
};

typedef struct {
	sse_tstree_node_t			*root;
	sse_uint_t					word_count;
	sse_uint_t					node_count;
	sse_pool_t					*pool;
	sse_log_t					*log;
} sse_tstree_t;

sse_tstree_t * sse_tstree_create(sse_pool_t *pool, sse_log_t *log);
void sse_tstree_insert(sse_tstree_t *tree, sse_str_t *key, void *data);
void * sse_tstree_search(sse_tstree_t *tree, sse_str_t *key);

#endif /* _SSE_TSTREE_H_INCLUDED_ */
