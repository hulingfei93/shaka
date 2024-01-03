#ifndef _SSE_TERM_DICT_H_INCLUDED_
#define _SSE_TERM_DICT_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

#define SSE_TERM_DICT_VECTOR_SIZE           100
#define SSE_TERM_DICT_VECTOR_LARGE_SIZE     100000
#define SSE_TERM_DICT_STACK_SIZE            100

sse_rbtree_t *sse_term_dict_create(sse_pool_t *pool, sse_log_t *log);
void sse_term_dict_add(sse_rbtree_t *term_dict, sse_str_t *term, sse_off_t start_offset, size_t len, sse_id_t doc_id);
void sse_term_dict_debug(sse_rbtree_t *term_dict);
void sse_term_dict_save(sse_rbtree_t *term_dict, const char* index_dir_path, sse_index_build_counter_t *counter);
size_t sse_term_compute_prefix(sse_str_t *str1, sse_str_t *str2);

#endif /* _SSE_TERM_DICT_H_INCLUDED_ */
