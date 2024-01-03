#ifndef _SSE_HASH_H_INCLUDED_
#define _SSE_HASH_H_INCLUDED_

#include <sse_core.h>

typedef struct sse_hash_kv_s    sse_hash_kv_t;

struct sse_hash_kv_s {
    sse_str_t                   *key;
    void                        *value;
    sse_hash_kv_t               *next;
};

typedef struct {
    sse_hash_kv_t               **buckets;
    size_t                      bucket_size;
    sse_uint_t                  kv_count;
    sse_uint_t                  conflict_count;
    sse_pool_t                  *pool;
    sse_log_t                   *log;
} sse_hash_t;


sse_hash_t *sse_hash_create(size_t size, sse_pool_t *pool, sse_log_t *log);
sse_bool_t sse_hash_contains(sse_hash_t *hash_table, sse_str_t *key);
sse_bool_t sse_hash_add(sse_hash_t *hash_table, sse_str_t *key, void *value);
void *sse_hash_find(sse_hash_t *hash_table, sse_str_t *key);
void sse_hash_debug_info(sse_hash_t *hash_table);

#endif /* _SSE_HASH_H_INCLUDED_ */
