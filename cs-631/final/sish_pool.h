/*
 * sish_pool.h
 */

#ifndef _SISH_POOL_H_INCLUDED_
#define _SISH_POOL_H_INCLUDED_

#include <sish_core.h>

typedef struct sish_pool_small_block_s  sish_pool_small_block_t;
typedef struct sish_pool_large_block_s  sish_pool_large_block_t;

struct sish_pool_small_block_s {
    char                        *last;
    char                        *end;
    sish_pool_small_block_t     *next;
};

struct sish_pool_large_block_s {
    sish_pool_large_block_t     *next;
    void                        *data;
};

typedef struct {
    sish_pool_small_block_t     *small;
    size_t                      small_block_size;
    sish_uint_t                 small_block_count;
    sish_pool_small_block_t     *current;
    sish_pool_large_block_t     *large;
} sish_pool_t;


sish_pool_t *sish_pool_create(size_t size);
void sish_pool_destory(sish_pool_t *pool);
void *sish_pool_alloc(sish_pool_t *pool, size_t size);
void *sish_pool_calloc(sish_pool_t *pool, size_t size);
void sish_pool_debug_info(sish_pool_t *pool);

#endif /* _SISH_POOL_H_INCLUDED_ */
