/*
 * ls_pool.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

static void *ls_pool_alloc_small(ls_pool_t *pool, ls_size_t size);
static void *ls_pool_alloc_large(ls_pool_t *pool, ls_size_t size);

ls_pool_t *
ls_pool_create(ls_size_t size)
{
	ls_pool_t	*pool;
	ls_size_t	small_block_alloc_size, small_block_size;

	pool = (ls_pool_t *)ls_alloc(sizeof(ls_pool_t));
	if (!pool) {
		return LS_NULL;
	}

	small_block_size = sizeof(ls_pool_small_block_t);
	small_block_alloc_size = small_block_size + size;
	pool->small = (ls_pool_small_block_t *)ls_alloc(small_block_alloc_size);

	if (pool->small == LS_NULL) {
		ls_free(pool);
		return LS_NULL;
	}

	pool->small->last = (char *)(pool->small) + small_block_size;
	pool->small->end = (char *)(pool->small) + small_block_alloc_size;
	pool->small->next = LS_NULL;

	pool->small_block_size = size;
	pool->small_block_count = 1;
	pool->current = pool->small;
	pool->large = LS_NULL;

	return pool;
}

void
ls_pool_destory(ls_pool_t *pool)
{
	ls_pool_small_block_t	*current_small, *next_small;
	ls_pool_large_block_t	*current_large, *next_large;

	/* free small block chain */
	next_small = pool->small;
	while (next_small) {
		current_small = next_small;
		next_small = next_small->next;
		ls_free(current_small);
	}
	
	/* free large block chain */
	next_large = pool->large;
	while (next_large) {
		current_large = next_large;
		next_large = next_large->next;
		ls_free(current_large);
	}

	/* free pool */
	ls_free(pool);
}

void *
ls_pool_alloc(ls_pool_t *pool, ls_size_t size)
{
	char *mem;
	ls_pool_small_block_t *c;
	ls_uint_t i;

	if (size > pool->small_block_size) {
		return ls_pool_alloc_large(pool, size);
	}
	else {
		for (i = 0, c = pool->current; i < pool->small_block_count; ++i) {
			mem = c->last;
			/* find the small block with enough size*/
			if ((ls_size_t)(c->end - c->last) >= size) {
				c->last += size;
				if (pool->current != c) {	/* update current small block */
					pool->current = c;
				}

				return mem;
			}
			/* try the next small block */
			if (c->next != LS_NULL) {
				c = c->next;
			}
			else {
				c = pool->small;
			}
		}
		return ls_pool_alloc_small(pool, size);
	}
}


void *
ls_pool_calloc(ls_pool_t *pool, ls_size_t size)
{
	void *mem;
	
	mem = ls_pool_alloc(pool, size);
	if (!mem) {
		return LS_NULL;
	}
	
	ls_memzero(mem, size);
	
	return mem;
}

static void *
ls_pool_alloc_small(ls_pool_t *pool, ls_size_t size)
{
	ls_pool_small_block_t	*small_mem, *next_small_block;
	ls_size_t				small_block_alloc_size, small_block_size;
	void					*mem;

	small_block_size = sizeof(ls_pool_small_block_t);
	small_block_alloc_size = small_block_size + pool->small_block_size;
	small_mem = (ls_pool_small_block_t *)ls_alloc(small_block_alloc_size);

	if (!small_mem) {
		return LS_NULL;
	}

	small_mem->last = (char *)(small_mem) + small_block_size;
	small_mem->end = (char *)(small_mem) + small_block_alloc_size;
	
	/* insert the new small block into the chain of pool */
	next_small_block = pool->current->next;
	small_mem->next = next_small_block;
	pool->current->next = small_mem;
    
	/* set the current small block to the new small block */
	pool->current = small_mem;

	++(pool->small_block_count);	/* plus 1 small block count */

	/* allocate mem in small block */
	mem = small_mem->last;
	small_mem->last += size;

	return mem;
}


static void *
ls_pool_alloc_large(ls_pool_t *pool, ls_size_t size)
{
	ls_pool_large_block_t	*large_mem;
	ls_size_t				large_block_size, large_block_alloc_size;

	large_block_size = sizeof(ls_pool_large_block_t);
	large_block_alloc_size = large_block_size + size;
	large_mem  = (ls_pool_large_block_t *)ls_alloc(large_block_alloc_size);
	
	if (!large_mem) {
		return LS_NULL;
	}

	large_mem->data = (char *)(large_mem) + large_block_size;
	large_mem->next = pool->large;
	pool->large = large_mem;

	return large_mem->data;
}

void ls_pool_debug_info(ls_pool_t *pool)
{
	ls_pool_small_block_t	*s;
	ls_pool_large_block_t	*l;
	ls_uint_t				small_count, large_count;

	printf("*******************************\n");
	printf("small block size: %d\n", (ls_int_t)(pool->small_block_size));
	printf("small block count: %d\n", (ls_int_t)(pool->small_block_count));
	
	small_count = 0;
	for (s = pool->small; s; s = s->next) {
		++small_count;
		printf("free size: %d bytes\n", (ls_int_t)(s->end - s->last));
	}
	printf("small count: %d\n", small_count);

	large_count = 0;
	for (l = pool->large; l; l = l->next) {
		++large_count;
	}
	printf("large count: %d\n", large_count);
}

