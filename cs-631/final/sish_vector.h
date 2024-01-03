/*
 * sish_vector.h
 */

#ifndef _SISH_VECTOR_H_INCLUDED_
#define _SISH_VECTOR_H_INCLUDED_

#include <sish_core.h>

typedef struct {
	sish_size_t			elem_size;		/* element size */
	sish_uint_t			size;			/* current count of element */
	sish_uint_t			capacity;		/* max capacity of storing elements */
	void				*data;			/* data storage */
	sish_pool_t			*pool;
} sish_vector_t;

sish_vector_t *sish_vector_create(size_t elem_size, sish_uint_t capacity, sish_pool_t *pool);
void sish_vector_push_back(sish_vector_t *vector, void *elem);
void *sish_vector_get(sish_vector_t *vector, sish_uint_t i);
void sish_vector_set(sish_vector_t *vector, sish_uint_t i, void *elem);
void *sish_vector_top(sish_vector_t *vector);
void sish_vector_pop(sish_vector_t *vector);
sish_bool_t sish_vector_is_empty(sish_vector_t *vector);
void sish_vector_cleanup(sish_vector_t *vector);
void sish_vector_reverse(sish_vector_t *vector);

#endif /* _SISH_VECTOR_H_INCLUDED_ */
