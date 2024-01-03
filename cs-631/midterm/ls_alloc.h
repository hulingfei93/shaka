/*
 * ls_alloc.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_ALLOC_H_INCLUDED_
#define _LS_ALLOC_H_INCLUDED_

#include <ls_core.h>

void *ls_alloc(size_t size);
void *ls_calloc(size_t size);
void *ls_memalign(size_t alignment, size_t size);

#define ls_free(p)      free(p)

#endif /* _LS_ALLOC_H_INCLUDED_ */
