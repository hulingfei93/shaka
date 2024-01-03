/*
 * sish_pool.c
 */

#include <sish_core.h>

static void *sish_pool_alloc_small(sish_pool_t *pool, sish_size_t size);
static void *sish_pool_alloc_large(sish_pool_t *pool, sish_size_t size);

sish_pool_t *
sish_pool_create(sish_size_t size)
{
    sish_pool_t     *pool;
    sish_size_t     small_block_alloc_size, small_block_size;

    pool = (sish_pool_t *)sish_alloc(sizeof(sish_pool_t));
    if (!pool) {
        return SISH_NULL;
    }

    small_block_size = sizeof(sish_pool_small_block_t);
    small_block_alloc_size = small_block_size + size;
    pool->small = (sish_pool_small_block_t *)sish_alloc(small_block_alloc_size);

    if (pool->small == SISH_NULL) {
        sish_free(pool);
        return SISH_NULL;
    }

    pool->small->last = (char *)(pool->small) + small_block_size;
    pool->small->end = (char *)(pool->small) + small_block_alloc_size;
    pool->small->next = SISH_NULL;

    pool->small_block_size = size;
    pool->small_block_count = 1;
    pool->current = pool->small;
    pool->large = SISH_NULL;

    return pool;
}

void
sish_pool_destory(sish_pool_t *pool)
{
    sish_pool_small_block_t     *current_small, *next_small;
    sish_pool_large_block_t     *current_large, *next_large;

    /* free small block chain */
    next_small = pool->small;
    while (next_small) {
        current_small = next_small;
        next_small = next_small->next;
        sish_free(current_small);
    }
    
    /* free large block chain */
    next_large = pool->large;
    while (next_large) {
        current_large = next_large;
        next_large = next_large->next;
        sish_free(current_large);
    }

    /* free pool */
    sish_free(pool);
}

void *
sish_pool_alloc(sish_pool_t *pool, sish_size_t size)
{
    char                        *mem;
    sish_pool_small_block_t     *c;
    sish_uint_t                 i;

    if (size > pool->small_block_size) {
        return sish_pool_alloc_large(pool, size);
    }
    else {
        for (i = 0, c = pool->current; i < pool->small_block_count; ++i) {
            mem = c->last;
            /* find the small block with enough size*/
            if ((sish_size_t)(c->end - c->last) >= size) {
                c->last += size;
                if (pool->current != c) {   /* update current small block */
                    pool->current = c;
                }

                return mem;
            }
            /* try the next small block */
            if (c->next != SISH_NULL) {
                c = c->next;
            }
            else {
                c = pool->small;
            }
        }
        return sish_pool_alloc_small(pool, size);
    }
}


void *
sish_pool_calloc(sish_pool_t *pool, sish_size_t size)
{
    void *mem;
    
    mem = sish_pool_alloc(pool, size);
    if (!mem) {
        return SISH_NULL;
    }
    
    memset(mem, 0, size);
    
    return mem;
}

static void *
sish_pool_alloc_small(sish_pool_t *pool, sish_size_t size)
{
    sish_pool_small_block_t     *small_mem, *next_small_block;
    sish_size_t                 small_block_alloc_size, small_block_size;
    void                        *mem;

    small_block_size = sizeof(sish_pool_small_block_t);
    small_block_alloc_size = small_block_size + pool->small_block_size;
    small_mem = (sish_pool_small_block_t *)sish_alloc(small_block_alloc_size);

    if (!small_mem) {
        return SISH_NULL;
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
sish_pool_alloc_large(sish_pool_t *pool, sish_size_t size)
{
    sish_pool_large_block_t     *large_mem;
    sish_size_t                 large_block_size, large_block_alloc_size;

    large_block_size = sizeof(sish_pool_large_block_t);
    large_block_alloc_size = large_block_size + size;
    large_mem  = (sish_pool_large_block_t *)sish_alloc(large_block_alloc_size);
    
    if (!large_mem) {
        return SISH_NULL;
    }

    large_mem->data = (char *)(large_mem) + large_block_size;
    large_mem->next = pool->large;
    pool->large = large_mem;

    return large_mem->data;
}

void sish_pool_debug_info(sish_pool_t *pool)
{
    sish_pool_small_block_t     *s;
    sish_pool_large_block_t     *l;
    sish_uint_t                 small_count, large_count;

    printf("*******************************\n");
    printf("small block size: %d\n", (sish_int_t)(pool->small_block_size));
    printf("small block count: %d\n", (sish_int_t)(pool->small_block_count));
    
    small_count = 0;
    for (s = pool->small; s; s = s->next) {
        ++small_count;
        printf("free size: %d bytes\n", (sish_int_t)(s->end - s->last));
    }
    printf("small count: %d\n", small_count);

    large_count = 0;
    for (l = pool->large; l; l = l->next) {
        ++large_count;
    }
    printf("large count: %d\n", large_count);
}

