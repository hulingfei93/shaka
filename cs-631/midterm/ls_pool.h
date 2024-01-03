/*
 * ls_pool.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_POOL_H_INCLUDED_
#define _LS_POOL_H_INCLUDED_

#include <ls_core.h>

typedef struct ls_pool_small_block_s    ls_pool_small_block_t;
typedef struct ls_pool_large_block_s    ls_pool_large_block_t;

struct ls_pool_small_block_s {
    char                        *last;
    char                        *end;
    ls_pool_small_block_t       *next;
};

struct ls_pool_large_block_s {
    ls_pool_large_block_t       *next;
    void                        *data;
};

typedef struct {
    ls_pool_small_block_t       *small;
    size_t                      small_block_size;
    ls_uint_t                   small_block_count;
    ls_pool_small_block_t       *current;
    ls_pool_large_block_t       *large;
} ls_pool_t;


ls_pool_t *ls_pool_create(size_t size);
void ls_pool_destory(ls_pool_t *pool);
void *ls_pool_alloc(ls_pool_t *pool, size_t size);
void *ls_pool_calloc(ls_pool_t *pool, size_t size);
void ls_pool_debug_info(ls_pool_t *pool);

#endif /* _LS_POOL_H_INCLUDED_ */
