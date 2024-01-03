#ifndef _SSE_INDEX_MERGE_H_INCLUDED_
#define _SSE_INDEX_MERGE_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

typedef struct {
    sse_uint_t      index_files_base;
    sse_uint_t      index_files_num;
    sse_pool_t      *pool;
    sse_log_t       *log;
} sse_index_merger_t;

sse_index_merger_t *sse_index_merger_create(sse_uint_t index_files_base, sse_uint_t index_files_num, sse_pool_t *pool, sse_log_t *log);
void sse_index_merge(sse_index_merger_t *merger);

#endif /* _SSE_INDEX_MERGE_H_INCLUDED_ */
