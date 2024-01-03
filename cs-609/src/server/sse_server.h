#ifndef _SSE_SERVER_H_INCLUDED_
#define _SSE_SERVER_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

typedef struct {
	sse_freq_t		doc_freq;
	sse_off_t		offset;
} sse_index_term_data_t;

typedef struct {
	sse_uint_t		low;
	sse_uint_t		high;
} sse_index_term_dict_range_t;

#include <sse_server_config.h>
#include <sse_server_process.h>
#include <sse_server_search.h>

#endif /* _SSE_SERVER_H_INCLUDED_ */
