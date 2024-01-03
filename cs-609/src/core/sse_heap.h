#ifndef _SSE_HEAP_H_INCLUDED_
#define _SSE_HEAP_H_INCLUDED_

#include <sse_core.h>

#define sse_heap_parent(i)      ((i) - 1) / 2
#define sse_heap_left(i)        (i) * 2 + 1
#define sse_heap_right(i)       (i) * 2 + 2

typedef struct {
    float                       score;
    void                        *data;
} sse_heap_item_t;

typedef struct {
    sse_heap_item_t             *items;
    sse_uint_t                  size;
    sse_uint_t                  capacity;
    sse_pool_t                  *pool;
    sse_log_t                   *log;
} sse_heap_t;

sse_heap_t * sse_heap_create(sse_uint_t capacity, sse_pool_t *pool, sse_log_t *log);
void  sse_heap_min_sort(sse_heap_t *heap);
void  sse_heap_min_insert(sse_heap_t *heap, float score, void *data);

#endif /* _SSE_HEAP_H_INCLUDED_ */
