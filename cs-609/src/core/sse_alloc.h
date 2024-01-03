#ifndef _SSE_ALLOC_H_INCLUDED_
#define _SSE_ALLOC_H_INCLUDED_

#include <sse_core.h>

void *sse_alloc(size_t size, sse_log_t *log);
void *sse_calloc(size_t size, sse_log_t *log);
void *sse_memalign(size_t alignment, size_t size, sse_log_t *log);

#define sse_free(p)     free(p)

#endif /* _SSE_ALLOC_H_INCLUDED_ */
