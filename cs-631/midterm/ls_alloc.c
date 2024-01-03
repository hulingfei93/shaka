/*
 * ls_alloc.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

void *ls_alloc(size_t size);
void *ls_calloc(size_t size);
void *ls_memalign(size_t alignment, size_t size);

void *
ls_alloc(size_t size)
{
	void *p;

	p = malloc(size);
	if (p == LS_NULL) {
		ls_log_err_exit("ls: malloc(%uz) error", size);
	}

	return p;
}

void *
ls_calloc(size_t size)
{
	void *p;

	p = ls_alloc(size);
	ls_memzero(p, size);

	return p;
}

void *
ls_memalign(size_t alignment, size_t size)
{
	void *p;
	int err;
	
	err = posix_memalign(&p, alignment, size);

	if (err) {
		ls_log_err_exit("ls: posix_memalign(%uz, %uz) error", alignment, size);
		p = LS_NULL;
	}

	return p;

}
