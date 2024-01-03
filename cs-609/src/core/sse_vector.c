#include <sse_core.h>

sse_vector_t *
sse_vector_create(size_t elem_size, sse_uint_t capacity, sse_pool_t *pool, sse_log_t *log)
{
	sse_vector_t *vector;

	vector = (sse_vector_t *)sse_pool_alloc(pool, sizeof(sse_vector_t));
	if(!vector) {
		return SSE_NULL;
	}

	vector->elem_size = elem_size;
	vector->size = 0;
	vector->capacity = capacity;
	vector->data = (void *)sse_pool_alloc(pool, vector->capacity * vector->elem_size);

	vector->pool = pool;
	vector->log = log;

	return vector;
}

void
sse_vector_push_back(sse_vector_t *vector, void *elem)
{
	void *addr;

	if (vector->size == vector->capacity) {
		void *old_data = vector->data;
		size_t old_size = vector->capacity * vector->elem_size;
		vector->data = (void *)sse_pool_alloc(vector->pool, 2 * old_size);
		sse_memcpy(vector->data, old_data, old_size);
		vector->capacity *= 2;
	}

	addr = (char *)(vector->data) + vector->elem_size * vector->size;
	sse_memcpy(addr, elem, vector->elem_size);
	++vector->size;
}

void *
sse_vector_get_at(sse_vector_t *vector, sse_uint_t i)
{
	void *addr;
	
	if(i >= vector->size) {
		sse_log(SSE_LOG_FATAL, vector->log, SSE_BOOL_FALSE, "Array overflow - index: %d", i);
	}

	addr = (char*)(vector->data) + vector->elem_size * i;

	return addr;

}

void *
sse_vector_top(sse_vector_t *vector)
{
	void *addr;

	if (vector->size == 0)
	  return SSE_NULL;

	addr = (char *)(vector->data) + vector->elem_size * (vector->size - 1);

	return addr;
}

void
sse_vector_pop(sse_vector_t *vector)
{
	if(vector->size > 0) {
		--vector->size;
	}
}

sse_bool_t
sse_vector_is_empty(sse_vector_t *vector)
{
	return vector->size == 0 ? SSE_BOOL_TRUE : SSE_BOOL_FALSE;
}

void
sse_vector_cleanup(sse_vector_t *vector)
{
	vector->size = 0;
}
