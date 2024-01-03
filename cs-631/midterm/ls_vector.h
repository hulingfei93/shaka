/*
 * ls_vector.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_VECTOR_H_INCLUDED_
#define _LS_VECTOR_H_INCLUDED_

#include <ls_core.h>

typedef struct {
    size_t              elem_size;      /* element size */
    ls_uint_t           size;           /* current count of element */
    ls_uint_t           capacity;       /* max capacity of storing elements */
    void                *data;          /* data storage */
    ls_pool_t           *pool;
} ls_vector_t;

ls_vector_t *ls_vector_create(size_t elem_size, ls_uint_t capacity, ls_pool_t *pool);
void ls_vector_push_back(ls_vector_t *vector, void *elem);
void *ls_vector_get(ls_vector_t *vector, ls_uint_t i);
void ls_vector_set(ls_vector_t *vector, ls_uint_t i, void *elem);
void *ls_vector_top(ls_vector_t *vector);
void ls_vector_pop(ls_vector_t *vector);
ls_bool_t ls_vector_is_empty(ls_vector_t *vector);
void ls_vector_cleanup(ls_vector_t *vector);
void ls_vector_reverse(ls_vector_t *vector);

#endif /* _LS_VECTOR_H_INCLUDED_ */
