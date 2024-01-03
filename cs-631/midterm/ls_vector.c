/*
 * ls_vector.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

ls_vector_t *
ls_vector_create(size_t elem_size, ls_uint_t capacity, ls_pool_t *pool)
{
	ls_vector_t *vector;

	vector = (ls_vector_t *)ls_pool_alloc(pool, sizeof(ls_vector_t));
	if(!vector) {
		return LS_NULL;
	}

	vector->elem_size = elem_size;
	vector->size = 0;
	vector->capacity = capacity;
	vector->data = (void *)ls_pool_alloc(pool, vector->capacity * vector->elem_size);

	vector->pool = pool;

	return vector;
}

void
ls_vector_push_back(ls_vector_t *vector, void *elem)
{
	void *addr;

	if (vector->size == vector->capacity) {
		void *old_data = vector->data;
		size_t old_size = vector->capacity * vector->elem_size;
		vector->data = (void *)ls_pool_alloc(vector->pool, 2 * old_size);
		ls_memcpy(vector->data, old_data, old_size);
		vector->capacity *= 2;
	}

	addr = (char *)(vector->data) + vector->elem_size * vector->size;
	ls_memcpy(addr, elem, vector->elem_size);
	++vector->size;
}

void *
ls_vector_get(ls_vector_t *vector, ls_uint_t i)
{
	void *addr;
	
	if (i >= vector->size) {
		ls_log_err_exit("ls_vector_get overflow - size: %d, index: %d", vector->size, i);
	}

	addr = (char *)(vector->data) + vector->elem_size * i;

	return addr;

}

void
ls_vector_set(ls_vector_t *vector, ls_uint_t i, void *elem)
{
	void *addr;

	if (i >= vector->size) {
		ls_log_err_exit("ls_vector_set overflow - size: %d, index: %d", vector->size, i);
	}

	addr = (char *)(vector->data) + vector->elem_size * i;
	ls_memcpy(addr, elem, vector->elem_size);
}

void *
ls_vector_top(ls_vector_t *vector)
{
	void *addr;

	if (vector->size == 0)
	  return LS_NULL;

	addr = (char *)(vector->data) + vector->elem_size * (vector->size - 1);

	return addr;
}

void
ls_vector_pop(ls_vector_t *vector)
{
	if(vector->size > 0) {
		--vector->size;
	}
}

ls_bool_t
ls_vector_is_empty(ls_vector_t *vector)
{
	return vector->size == 0 ? LS_BOOL_TRUE : LS_BOOL_FALSE;
}

void
ls_vector_cleanup(ls_vector_t *vector)
{
	vector->size = 0;
}

void
ls_vector_reverse(ls_vector_t *vector)
{
	void		*mem_tmp, *addr1, *addr2;
	ls_uint_t	i, j;

	if (vector->size == 0) {
		return;
	}

	mem_tmp = ls_alloc(vector->elem_size);
		
	for (i = 0, j = vector->size - 1; i < j; ++i, --j) {
		addr1 = (char *)(vector->data) + vector->elem_size * i;
		addr2 = (char *)(vector->data) + vector->elem_size * j;
		ls_memcpy(mem_tmp, addr1, vector->elem_size);
		ls_memcpy(addr1, addr2, vector->elem_size);
		ls_memcpy(addr2, mem_tmp, vector->elem_size);
	} 
	ls_free(mem_tmp);
}
