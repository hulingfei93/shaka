#ifndef _SSE_VECTOR_H_INCLUDED_
#define _SSE_VECTOR_H_INCLUDED_

#include <sse_core.h>

typedef struct {
    size_t              elem_size;      /* element size */
    sse_uint_t          size;           /* current count of element */
    sse_uint_t          capacity;       /* max capacity of storing elements */
    void                *data;          /* data storage */
    sse_pool_t          *pool;
    sse_log_t           *log;
} sse_vector_t;

sse_vector_t *sse_vector_create(size_t elem_size, sse_uint_t capacity, sse_pool_t *pool, sse_log_t *log);
void sse_vector_push_back(sse_vector_t *vector, void *elem);
void *sse_vector_get_at(sse_vector_t *vector, sse_uint_t i);
void *sse_vector_top(sse_vector_t *vector);
void sse_vector_pop(sse_vector_t *vector);
sse_bool_t sse_vector_is_empty(sse_vector_t *vector);
void sse_vector_cleanup(sse_vector_t *vector);

#endif /* _SSE_VECTOR_H_INCLUDED_ */
