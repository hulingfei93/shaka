#ifndef _SSE_RBTREE_H_INCLUDED_
#define _SSE_RBTREE_H_INCLUDED_

#include <sse_core.h>

#define SSE_RBTREE_BLACK(node)          ((node)->color = 0)
#define SSE_RBTREE_RED(node)            ((node)->color = 1)
#define SSE_RBTREE_IS_BLACK(node)       (!(node)->color)
#define SSE_RBTREE_IS_RED(node)         ((node)->color)
#define SSE_RBTREE_COPY_COLOR(n1, n2)   ((n1)->color = (n2)->color)

typedef struct sse_rbtree_node_s    sse_rbtree_node_t;

struct sse_rbtree_node_s {
    sse_str_t                   *key;
    void                        *value;
    sse_rbtree_node_t           *left;
    sse_rbtree_node_t           *right;
    sse_rbtree_node_t           *parent;
    char                        color;
};

typedef struct {
    sse_rbtree_node_t           *root;
    sse_rbtree_node_t           *sentinel;
    sse_uint_t                  count;
    sse_pool_t                  *pool;
    sse_log_t                   *log;
} sse_rbtree_t;

sse_rbtree_t *sse_rbtree_create(sse_pool_t *pool, sse_log_t *log);
void sse_rbtree_insert(sse_rbtree_t *tree, sse_str_t *key, void *value);
sse_bool_t sse_rbtree_contains(sse_rbtree_t *tree, sse_str_t *key);
void *sse_rbtree_find(sse_rbtree_t *tree, sse_str_t *key);
void sse_rbtree_inorder(sse_rbtree_t *tree);

#endif /* _SSE_RBTREE_H_INCLUDED_ */
