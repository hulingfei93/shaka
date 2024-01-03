#include <sse_core.h>
#include <sse_index.h>

static void sse_term_dict_output(sse_term_dict_index_data_t *index_data);
static void sse_term_dict_save_imp(sse_term_dict_index_data_t *data_index, sse_index_build_counter_t *counter, sse_index_file_set_t *index_files);

sse_rbtree_t *
sse_term_dict_create(sse_pool_t *pool, sse_log_t *log)
{
    sse_rbtree_t *term_dict;

    term_dict = sse_rbtree_create(pool, log);

    return term_dict;
}

void
sse_term_dict_add(sse_rbtree_t *term_dict, sse_str_t *term, sse_off_t start_offset, size_t len, sse_id_t doc_id)
{
    sse_str_t *new_term;
    sse_term_dict_index_data_t *index_data;

    index_data = (sse_term_dict_index_data_t *)sse_rbtree_find(term_dict, term);
    if (index_data == SSE_NULL) {
        /* create a new term */
        new_term = (sse_str_t *)sse_pool_alloc(term_dict->pool, sizeof(sse_str_t));
        new_term->data = (char *)sse_pool_alloc(term_dict->pool, term->len);
        sse_memcpy(new_term->data, term->data, term->len);
        new_term->len = term->len;

        /* create related index data for the new term */
        sse_term_docid_freq_t docid_freq;
        sse_term_offset_t   term_offset;

        index_data = (sse_term_dict_index_data_t *)sse_pool_alloc(term_dict->pool, sizeof(sse_term_dict_index_data_t));
        index_data->term = new_term;
        index_data->doc_freq = 1;
        
        /* fill out doc id and frequency */
        index_data->ids_freqs = sse_vector_create(sizeof(sse_term_docid_freq_t), SSE_TERM_DICT_VECTOR_SIZE, term_dict->pool, term_dict->log);
        docid_freq.doc_id = doc_id;
        docid_freq.freq = 1; 
        sse_vector_push_back(index_data->ids_freqs, &docid_freq);
        
        /* fill out offset of the term */
        index_data->offsets = sse_vector_create(sizeof(sse_term_offset_t), SSE_TERM_DICT_VECTOR_LARGE_SIZE, term_dict->pool, term_dict->log);
        term_offset.start = start_offset;
        term_offset.len = len;
        sse_vector_push_back(index_data->offsets, &term_offset);

        sse_rbtree_insert(term_dict, new_term, index_data);
    }
    else {
        sse_term_docid_freq_t   *last_docid_freq;
        sse_term_docid_freq_t   new_docid_freq;
        sse_term_offset_t       new_term_offset;
        
        last_docid_freq = (sse_term_docid_freq_t *)sse_vector_get_at(index_data->ids_freqs, index_data->ids_freqs->size - 1);

        if (last_docid_freq->doc_id == doc_id) {
            ++last_docid_freq->freq;
        }
        else {
            ++index_data->doc_freq;
            new_docid_freq.doc_id = doc_id;
            new_docid_freq.freq = 1;
            sse_vector_push_back(index_data->ids_freqs, &new_docid_freq);
        }
        
        new_term_offset.start = start_offset;
        new_term_offset.len = len;
        sse_vector_push_back(index_data->offsets, &new_term_offset);
    }
}

void
sse_term_dict_save(sse_rbtree_t *term_dict, const char* index_dir_path, sse_index_build_counter_t *counter)
{
    sse_vector_t            *stack;
    sse_rbtree_node_t       **temp;
    sse_index_file_set_t    index_files;
    char                    index_file_path[SSE_PATH_MAX_SIZE];
    char                    term_dict_file_path[SSE_PATH_MAX_SIZE];
    char                    docid_freq_file_path[SSE_PATH_MAX_SIZE];
    char                    offset_file_path[SSE_PATH_MAX_SIZE];
    size_t                  index_file_path_offset;
    char                    count_buf[SSE_INT_MAX_SIZE];


    /* init counter */
    ++counter->index_file_count;
    counter->term_pos = 0;
    counter->prev_docid_freq_offset = 0;
    counter->prev_index_data = SSE_NULL;

    sse_log(SSE_LOG_INFO, term_dict->log, SSE_BOOL_FALSE, "Begin to build the index segment %u.", counter->index_file_count);

    sse_strcpy(index_file_path, index_dir_path);
    index_file_path_offset = sse_strlen(index_dir_path);
    if (index_file_path[index_file_path_offset - 1] != '/') {
        index_file_path[index_file_path_offset++] = '/';
    }
    
    snprintf(count_buf, SSE_INT_MAX_SIZE, "%d", counter->index_file_count);
    sse_strcpy(index_file_path + index_file_path_offset, count_buf);
    index_file_path_offset += sse_strlen(count_buf);

    /* create a term dictionary index file */
    sse_strcpy(index_file_path + index_file_path_offset, CONFIG.INDEX_TERM_DICT_FILE_EXTENSION);
    sse_strcpy(term_dict_file_path, index_file_path);
    index_files.term_dict_file = fopen(term_dict_file_path, "w");
    index_files.term_dict_file_path = term_dict_file_path;
    /* write the term count and block size of term dictionary index file */
    sse_stdio_write_uint(term_dict->count, index_files.term_dict_file);
    sse_stdio_write_vuint(CONFIG.INDEX_TERM_DICT_FILE_BLOCK_SIZE, index_files.term_dict_file);
    
    /* create a docid frequency index file */
    sse_strcpy(index_file_path + index_file_path_offset, CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION);
    sse_strcpy(docid_freq_file_path, index_file_path);
    index_files.docid_freq_file = fopen(docid_freq_file_path, "w");
    index_files.docid_freq_file_path = docid_freq_file_path;

    /* create a offset index file */
    sse_strcpy(index_file_path + index_file_path_offset, CONFIG.INDEX_OFFSET_FILE_EXTENSION);
    sse_strcpy(offset_file_path, index_file_path);
    index_files.offset_file = fopen(offset_file_path, "w");
    index_files.offset_file_path = offset_file_path;
    
    /* traverse term dictionary in alphabetic order */
    stack = sse_vector_create(sizeof(sse_rbtree_node_t *), SSE_TERM_DICT_STACK_SIZE, term_dict->pool, term_dict->log);
    sse_vector_push_back(stack, &(term_dict->root));

    while (!sse_vector_is_empty(stack)) {
        temp = (sse_rbtree_node_t **)sse_vector_top(stack);
        while (*temp != term_dict->sentinel) {
            sse_vector_push_back(stack, &((*temp)->left));
            temp = (sse_rbtree_node_t **)sse_vector_top(stack);
        }
        
        sse_vector_pop(stack);
        if (!sse_vector_is_empty(stack)) {
            temp = (sse_rbtree_node_t **)sse_vector_top(stack);
            sse_vector_pop(stack);
            
            sse_term_dict_save_imp((sse_term_dict_index_data_t *)((*temp)->value), counter, &index_files);
    
            sse_vector_push_back(stack, &((*temp)->right));
        }
    }

    fclose(index_files.term_dict_file);
    fclose(index_files.docid_freq_file);
    fclose(index_files.offset_file);

    sse_log(SSE_LOG_INFO, term_dict->log, SSE_BOOL_FALSE, "Done!");
}

static void
sse_term_dict_save_imp(sse_term_dict_index_data_t *data_index, sse_index_build_counter_t *counter, sse_index_file_set_t *index_files)
{
    sse_str_t                   *prev_term_dict, *term_dict;
    size_t                      prefix_len, diff_len;
    sse_uint_t                  i, j, m;
    sse_vector_t                *ids_freqs, *offsets;
    sse_term_docid_freq_t       *docid_freq;
    sse_term_offset_t           *term_offset;
    sse_off_t                   cur_docid_freq_offset, prev_offset_offset, cur_offset_offset, prev_offset_start;
    sse_id_t                    prev_doc_id;

    if (counter->prev_index_data != SSE_NULL) {
        prev_term_dict = counter->prev_index_data->term;
    }
    term_dict = data_index->term;

    /******************term dictionary index file************************/
    /* head term in the block */
    if (counter->term_pos % CONFIG.INDEX_TERM_DICT_FILE_BLOCK_SIZE == 0) {
        /* [string length][string] */
        sse_stdio_write_vuint(term_dict->len, index_files->term_dict_file);
        sse_stdio_write(term_dict->data, term_dict->len, index_files->term_dict_file);
        /* [doc frequency] */
        sse_stdio_write_vuint(data_index->doc_freq, index_files->term_dict_file);
        /* [offset](d-gap) in docid-freq index file */
        cur_docid_freq_offset = ftello(index_files->docid_freq_file);
        sse_stdio_write_vuint(cur_docid_freq_offset, index_files->term_dict_file);
    }
    /* no head term in the block */
    else {
        /* [prefix length][suffix length][suffix] */
        prefix_len = sse_term_compute_prefix(prev_term_dict, term_dict);
        sse_stdio_write_vuint(prefix_len, index_files->term_dict_file);
        diff_len = term_dict->len - prefix_len;
        sse_stdio_write_vuint(diff_len, index_files->term_dict_file);
        sse_stdio_write(term_dict->data + prefix_len, diff_len, index_files->term_dict_file);
        /* [doc frequency] */
        sse_stdio_write_vuint(data_index->doc_freq, index_files->term_dict_file);
        /* [offset](d-gap) in docid-freq index file */
        cur_docid_freq_offset = ftello(index_files->docid_freq_file);
        sse_stdio_write_vuint(cur_docid_freq_offset - counter->prev_docid_freq_offset, index_files->term_dict_file);
    }
    counter->prev_docid_freq_offset = cur_docid_freq_offset;
    
    /*****************docid-freq index file*********************/
    ids_freqs = data_index->ids_freqs;
    offsets = data_index->offsets;
    m = 0;
    for (i = 0; i < ids_freqs->size; ++i) {
        docid_freq = (sse_term_docid_freq_t *)sse_vector_get_at(ids_freqs, i);
        
        /* [doc id](d-gap) */
        if (i == 0) {
            sse_stdio_write_vuint(docid_freq->doc_id, index_files->docid_freq_file);
        }
        else {
            sse_stdio_write_vuint(docid_freq->doc_id - prev_doc_id, index_files->docid_freq_file);
        }
        prev_doc_id = docid_freq->doc_id;   /* set prev doc id */
        
        /* [term frequency] */
        sse_stdio_write_vuint(docid_freq->freq, index_files->docid_freq_file);

        /* [weight] */
        sse_stdio_write_float(0.0f, index_files->docid_freq_file);

        /* [offset](d-gap) in offset index file */
        cur_offset_offset = ftello(index_files->offset_file);
        if (i == 0) {
            sse_stdio_write_vuint(cur_offset_offset, index_files->docid_freq_file);
        }
        else {
            sse_stdio_write_vuint(cur_offset_offset - prev_offset_offset, index_files->docid_freq_file);
        }
        prev_offset_offset = cur_offset_offset;

        /***************offset index file****************/
        for (j = 0; j < docid_freq->freq; ++j) {
            term_offset = (sse_term_offset_t *)sse_vector_get_at(offsets, m++);
            if (j == 0) {
                sse_stdio_write_vuint(term_offset->start, index_files->offset_file);
            }
            else {
                sse_stdio_write_vuint(term_offset->start - prev_offset_start, index_files->offset_file);
            }
            sse_stdio_write_vuint(term_offset->len, index_files->offset_file);
            prev_offset_start = term_offset->start;
        }
    }

    ++counter->term_pos;
    counter->prev_index_data = data_index;
}

size_t
sse_term_compute_prefix(sse_str_t *str1, sse_str_t *str2)
{
    size_t      min_len, prefix_len;
    char        *c1, *c2;

    min_len = sse_min(str1->len, str2->len);
    prefix_len = 0;
    c1 = str1->data;
    c2 = str2->data;

    while (prefix_len < min_len) {
        if (*c1++ != *c2++) {
            break;
        }
        ++prefix_len;
    }

    return prefix_len;
}

void
sse_term_dict_debug(sse_rbtree_t *term_dict)
{
    sse_vector_t            *stack;
    sse_rbtree_node_t       **temp;

    stack = sse_vector_create(sizeof(sse_rbtree_node_t *), SSE_TERM_DICT_STACK_SIZE, term_dict->pool, term_dict->log);
    sse_vector_push_back(stack, &(term_dict->root));

    printf("*********begin*********\n");

    while (!sse_vector_is_empty(stack)) {
        temp = (sse_rbtree_node_t **)sse_vector_top(stack);
        while (*temp != term_dict->sentinel) {
            sse_vector_push_back(stack, &((*temp)->left));
            temp = (sse_rbtree_node_t **)sse_vector_top(stack);
        }
        
        sse_vector_pop(stack);
        if (!sse_vector_is_empty(stack)) {
            temp = (sse_rbtree_node_t **)sse_vector_top(stack);
            sse_vector_pop(stack);
            
            sse_term_dict_output((sse_term_dict_index_data_t *)((*temp)->value));
    
            sse_vector_push_back(stack, &((*temp)->right));
        }
    }
}

static void
sse_term_dict_output(sse_term_dict_index_data_t *index_data)
{
    sse_uint_t                  i;
    sse_term_docid_freq_t       *docid_freq;
    sse_term_offset_t           *term_offset;

    write(SSE_FD_STDOUT, index_data->term->data, index_data->term->len);

    printf(" {doc_freq: %u}", (sse_uint_t)(index_data->doc_freq));

    sse_vector_t *ids_freqs = index_data->ids_freqs;
    printf(" {docid-freq:");
    for (i = 0; i < ids_freqs->size; ++i) {
        docid_freq = (sse_term_docid_freq_t *)sse_vector_get_at(ids_freqs, i);
        printf(" %u-%u", (sse_uint_t)(docid_freq->doc_id), (sse_uint_t)(docid_freq->freq));
    }
    printf("}");
    
    sse_vector_t *offsets = index_data->offsets;
    printf(" {offsets:");
    for (i = 0; i < offsets->size; ++i) {
        term_offset = (sse_term_offset_t *)sse_vector_get_at(offsets, i);
        printf(" %u-%u", (sse_uint_t)(term_offset->start), (sse_uint_t)(term_offset->len));
    }
    printf("}\n");
}
