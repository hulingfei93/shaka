#ifndef _SSE_SERVER_PROCESS_H_INCLUDED_
#define _SSE_SERVER_PROCESS_H_INCLUDED_

#include <sse_core.h>
#include <sse_server.h>

typedef struct {
    sse_uint_t      doc_count;
    sse_tstree_t    *tstree_term_dict;
    sse_hash_t      *stopwords_dict;
    sse_pool_t      *pool;
    sse_log_t       *log;
} sse_server_t;

sse_server_t * sse_server_create(sse_pool_t *pool, sse_log_t *log);
void sse_server_init(sse_server_t *server);
#endif /* _SSE_SERVER_PROCESS_H_INCLUDED_ */
