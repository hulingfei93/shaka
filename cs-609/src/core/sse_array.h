#if 0
#ifndef _SSE_VECTOR_H_INCLUDED_
#define _SSE_VECTOR_H_INCLUDED_

#include <sse_core.h>

typedef struct {
	const sse_uint_t	bucket_size;	/* fixed size of bucket */
	sse_uint_t			bucket_num;		/* initial number of bucket*/
	size_t				elem_size;		/* element size */
	size_t				size;			/* current count of element */
	size_t				capacity;		/* max capacity of storing elements */

	void				**buckets;		/* data storage */
} sse_vector_t

#endif /* _SSE_VECTOR_H_INCLUDED_ */
