/*
 * sish_alloc.h
 */

#ifndef _SISH_ALLOC_H_INCLUDED_
#define _SISH_ALLOC_H_INCLUDED_

#include <sish_core.h>

void *sish_alloc(size_t size);
void *sish_calloc(size_t size);

#define sish_free(p)        free(p)

#endif /* _SISH_ALLOC_H_INCLUDED_ */
