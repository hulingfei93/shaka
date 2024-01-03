#if 0
#include <sse_core.h>

sse_vector_t *
sse_vector_create(sse_uint_t bucket_size, sse_uint_t bucket_num, size_t elem_size, sse_pool_t *pool, sse_log_t *log)
{
	sse_vector_t *vector;

	vector = (sse_vector_t *)sse_pool_alloc(pool, sizeof(sse_vector_t));
	if(!vector) {
		return SSE_NULL;
	}

	vector->bucket_size = bucket_size;
	vector->bucket_num = bucket_num;
	vector->elem_size;
	vector->size = 0;
	vector->capacity = bucket_size * bucket_num;

	vector->buckets = (void **)sse_pool_alloc(pool, bucket_num * sizeof(void *));
}

void
sse_vector_push_back(sse_vector_t *vector, void *elem)
{
	sse_uint_t		bucket_index, array_index, vector_index;
	size_t			size;
	void			**bucket_addr 
	void			*addr;
	
	/* if the vector is full */
	if (size == capacity) {
		/* extend the buckets with the size of two times*/
		void **old_buckets = vector->buckets;
		size_t old_size = vector->bucket_num * sizeof(void *);
		vector->buckets = (void **)sse_pool_calloc(pool, 2*old_size);
		sse_memcpy(vector->buckets, old_buckets, old_size);
		
		vector->bucket_num *= 2;
		vector->capacity = bucket_size * bucket_num;
	}

	/* calculate the bucket index and array index */
	vector_index = vector->size;
	bucket_index = vector_index / vector->bucket_num;
	array_index = vector_index % (vector->bucket_size);

	bucket_addr = (char *)vector->buckets + bucket_index * sizeof(void *);
	/* alloc memory for destination bucket */
	if (*bucket_addr == SSE_NULL) {
		*bucket_addr = (void *)sse_pool_alloc(pool, vector->bucket_size * vector->elem_size);
	}
	addr = (char*)(*bucket_addr) + array_index * vector->elem_size;

	sse_memcpy(addr, elem, vector->elem_size);
	
	++vector->size;
}

void *
sse_vector_get_at(sse_vector_t *vector, sse_uint_t i)
{
	sse_uint_t		bucket_index, array_index, vector_index;
	void			**bucket_addr 
	void			*addr;
	
	if(i >= vector->size) {
		sse_log(SSE_LOG_FATAL, vector->log, SSE_BOOL_FALSE, "Array overflow - index: %d", i);
	}

	/* calculate the bucket index and array index */
	vector_index = i;
	bucket_index = vector_index / vector->bucket_num;
	array_index = vector_index % (vector->bucket_size);

	bucket_addr = (char *)vector->buckets + bucket_index * sizeof(void *);
	/* alloc memory for destination bucket */
	if (*bucket_addr == SSE_NULL) {
		return SSE_NULL;
	}
	addr = (char*)(*bucket_addr) + array_index * vector->elem_size;

	return addr;

}
#endif
