#include <sse_core.h>

static void *sse_pool_alloc_small(sse_pool_t *pool, size_t size);
static void *sse_pool_alloc_large(sse_pool_t *pool, size_t size);
/* static void sse_pool_current_shift(sse_pool_t *pool, sse_uint_t steps); */

sse_pool_t *
sse_pool_create(size_t size, sse_log_t *log)
{
    sse_pool_t *pool;
    size_t small_block_alloc_size, small_block_size;

    pool = (sse_pool_t *)sse_alloc(sizeof(sse_pool_t), log);
    if (!pool) {
        return SSE_NULL;
    }

    small_block_size = sizeof(sse_pool_small_block_t);
    small_block_alloc_size = small_block_size + size;
    pool->small = (sse_pool_small_block_t *)sse_alloc(small_block_alloc_size, log);

    if (pool->small == SSE_NULL) {
        sse_free(pool);
        return SSE_NULL;
    }

    pool->small->last = (char *)(pool->small) + small_block_size;
    pool->small->end = (char *)(pool->small) + small_block_alloc_size;
    pool->small->next = SSE_NULL;

    pool->small_block_size = size;
    pool->small_block_count = 1;
    pool->current = pool->small;
    pool->large = SSE_NULL;
    pool->log = log;

    return pool;
}

void
sse_pool_destory(sse_pool_t *pool)
{
    sse_pool_small_block_t *current_small, *next_small;
    sse_pool_large_block_t *current_large, *next_large;

    /* free small block chain */
    next_small = pool->small;
    while (next_small) {
        current_small = next_small;
        next_small = next_small->next;
        sse_free(current_small);
    }
    
    /* free large block chain */
    next_large = pool->large;
    while (next_large) {
        current_large = next_large;
        next_large = next_large->next;
        sse_free(current_large);
    }

    /* free pool */
    sse_free(pool);
}

void *
sse_pool_alloc(sse_pool_t *pool, size_t size)
{
    char *mem;
    sse_pool_small_block_t *c;
    sse_uint_t i;

    if (size > pool->small_block_size) {
        return sse_pool_alloc_large(pool, size);
    }
    else {
        for (i = 0, c = pool->current; i < pool->small_block_count; ++i) {
            mem = c->last;
            /* find the small block with enough size*/
            if ((size_t)(c->end - c->last) >= size) {
                c->last += size;
                if (pool->current != c) {   /* update current small block */
                    pool->current = c;
                }

                return mem;
            }
            /* try the next small block */
            if (c->next != SSE_NULL) {
                c = c->next;
            }
            else {
                c = pool->small;
            }
        }
        return sse_pool_alloc_small(pool, size);
    }
}


void *
sse_pool_calloc(sse_pool_t *pool, size_t size)
{
    void *mem;
    
    mem = sse_pool_alloc(pool, size);
    if (!mem) {
        return SSE_NULL;
    }
    
    sse_memzero(mem, size);
    
    return mem;
}

static void *
sse_pool_alloc_small(sse_pool_t *pool, size_t size)
{
    sse_pool_small_block_t *small_mem, *next_small_block;
    size_t small_block_alloc_size, small_block_size;
    void *mem;

    small_block_size = sizeof(sse_pool_small_block_t);
    small_block_alloc_size = small_block_size + pool->small_block_size;
    small_mem = (sse_pool_small_block_t *)sse_alloc(small_block_alloc_size, pool->log);

    if (!small_mem) {
        return SSE_NULL;
    }

    small_mem->last = (char *)(small_mem) + small_block_size;
    small_mem->end = (char *)(small_mem) + small_block_alloc_size;
    
    /* insert the new small block into the chain of pool */
    next_small_block = pool->current->next;
    small_mem->next = next_small_block;
    pool->current->next = small_mem;
    
    /* set the current small block to the new small block */
    pool->current = small_mem;

    ++(pool->small_block_count);    /* plus 1 small block count */

    /* allocate mem in small block */
    mem = small_mem->last;
    small_mem->last += size;

    return mem;
}


static void *
sse_pool_alloc_large(sse_pool_t *pool, size_t size)
{
    sse_pool_large_block_t *large_mem;
    size_t large_block_size, large_block_alloc_size;

    large_block_size = sizeof(sse_pool_large_block_t);
    large_block_alloc_size = large_block_size + size;
    large_mem  = (sse_pool_large_block_t *)sse_alloc(large_block_alloc_size, pool->log);
    
    if (!large_mem) {
        return SSE_NULL;
    }

    large_mem->data = (char *)(large_mem) + large_block_size;
    large_mem->next = pool->large;
    pool->large = large_mem;

    return large_mem->data;
}

void sse_pool_debug_info(sse_pool_t *pool)
{
    sse_pool_small_block_t *s;
    sse_pool_large_block_t *l;
    sse_uint_t small_count, large_count;

    printf("*******************************\n");
    printf("small block size: %d\n", (sse_int_t)(pool->small_block_size));
    printf("small block count: %d\n", (sse_int_t)(pool->small_block_count));
    
    small_count = 0;
    for (s = pool->small; s; s = s->next) {
        ++small_count;
        printf("free size: %d bytes\n", (sse_int_t)(s->end - s->last));
    }
    printf("small count: %d\n", small_count);

    large_count = 0;
    for (l = pool->large; l; l = l->next) {
        ++large_count;
    }
    printf("large count: %d\n", large_count);


}

/*
static void
sse_pool_current_shift(sse_pool_t *pool, sse_uint_t steps) 
{
    while (steps > 0) {
        if(pool->current->next) {
            pool->current = pool->current->next;
        }
        else {
            pool->current = pool->small;
        }
        --steps;
    }
}
*/

