#include <sse_core.h>

void *
sse_alloc(size_t size, sse_log_t *log)
{
	void *p;

	p = malloc(size);
	if (p == SSE_NULL) {
		sse_log(SSE_LOG_FATAL, log, SSE_BOOL_TRUE, "malloc(%uz) failed.", size);
	}

	return p;
}

void *
sse_calloc(size_t size, sse_log_t *log)
{
	void *p;

	p = sse_alloc(size, log);
	if (p) {
		sse_memzero(p, size);
	}

	return p;
}

void *
sse_memalign(size_t alignment, size_t size, sse_log_t *log)
{
	void *p;
	int err;
	
	err = posix_memalign(&p, alignment, size);

	if (err) {
		sse_log(SSE_LOG_FATAL, log, SSE_BOOL_FALSE, "posix_memalign(%uz, %uz) failed.", alignment, size);
		p = SSE_NULL;
	}

	return p;

}
