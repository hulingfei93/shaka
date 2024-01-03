/*
 * sish_alloc.c
 */

#include <sish_core.h>

void *sish_alloc(size_t size);
void *sish_calloc(size_t size);

void *
sish_alloc(size_t size)
{
	void *p;

	p = malloc(size);
	if (p == SISH_NULL) {
		sish_log_err_exit("sish: malloc(%uz) error\n", size);
	}

	return p;
}

void *
sish_calloc(size_t size)
{
	void *p;

	p = sish_alloc(size);
	memset(p, 0, size);

	return p;
}

