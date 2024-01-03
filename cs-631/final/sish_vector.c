/*
 * sish_vector.c
 */

#include <sish_core.h>

sish_vector_t *
sish_vector_create(size_t elem_size, sish_uint_t capacity, sish_pool_t *pool)
{
    sish_vector_t   *vector;

    vector = (sish_vector_t *)sish_pool_alloc(pool, sizeof(sish_vector_t));
    if(!vector) {
        return SISH_NULL;
    }

    vector->elem_size = elem_size;
    vector->size = 0;
    vector->capacity = capacity;
    vector->data = (void *)sish_pool_alloc(pool, vector->capacity * vector->elem_size);

    vector->pool = pool;

    return vector;
}

void
sish_vector_push_back(sish_vector_t *vector, void *elem)
{
    void *addr;

    if (vector->size == vector->capacity) {
        void *old_data = vector->data;
        size_t old_size = vector->capacity * vector->elem_size;
        vector->data = (void *)sish_pool_alloc(vector->pool, 2 * old_size);
        memcpy(vector->data, old_data, old_size);
        vector->capacity *= 2;
    }

    addr = (char *)(vector->data) + vector->elem_size * vector->size;
    memcpy(addr, elem, vector->elem_size);
    ++vector->size;
}

void *
sish_vector_get(sish_vector_t *vector, sish_uint_t i)
{
    void *addr;
    
    if (i >= vector->size) {
        sish_log_err_exit("sish_vector_get overflow - size: %d, index: %d", vector->size, i);
    }

    addr = (char *)(vector->data) + vector->elem_size * i;

    return addr;

}

void
sish_vector_set(sish_vector_t *vector, sish_uint_t i, void *elem)
{
    void *addr;

    if (i >= vector->size) {
        sish_log_err_exit("sish_vector_set overflow - size: %d, index: %d", vector->size, i);
    }

    addr = (char *)(vector->data) + vector->elem_size * i;
    memcpy(addr, elem, vector->elem_size);
}

void *
sish_vector_top(sish_vector_t *vector)
{
    void *addr;

    if (vector->size == 0)
      return SISH_NULL;

    addr = (char *)(vector->data) + vector->elem_size * (vector->size - 1);

    return addr;
}

void
sish_vector_pop(sish_vector_t *vector)
{
    if(vector->size > 0) {
        --vector->size;
    }
}

sish_bool_t
sish_vector_is_empty(sish_vector_t *vector)
{
    return vector->size == 0 ? true : false;
}

void
sish_vector_cleanup(sish_vector_t *vector)
{
    vector->size = 0;
}

void
sish_vector_reverse(sish_vector_t *vector)
{
    void            *mem_tmp, *addr1, *addr2;
    sish_uint_t     i, j;

    if (vector->size == 0) {
        return;
    }

    mem_tmp = sish_alloc(vector->elem_size);
        
    for (i = 0, j = vector->size - 1; i < j; ++i, --j) {
        addr1 = (char *)(vector->data) + vector->elem_size * i;
        addr2 = (char *)(vector->data) + vector->elem_size * j;
        memcpy(mem_tmp, addr1, vector->elem_size);
        memcpy(addr1, addr2, vector->elem_size);
        memcpy(addr2, mem_tmp, vector->elem_size);
    } 
    sish_free(mem_tmp);
}
