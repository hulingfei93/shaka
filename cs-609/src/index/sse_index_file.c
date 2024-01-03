#include <sse_core.h>
#include <sse_index.h>

sse_index_file_term_dict_t *
sse_index_file_term_dict_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_term_dict_t *term_dict;

    term_dict = (sse_index_file_term_dict_t *)sse_pool_alloc(pool, sizeof(sse_index_file_term_dict_t));
    if (!term_dict) {
        return SSE_NULL;
    }

    term_dict->term_num = 0;
    term_dict->term_count = 0;
    term_dict->term_block_size = 0;

    term_dict->prev_term = (sse_index_file_term_t *)sse_pool_alloc(pool, sizeof(sse_index_file_term_t));
    term_dict->prev_term->term.data = (char *)sse_pool_alloc(pool, SSE_TOKENIZER_TOKEN_BUF_SIZE);
    term_dict->prev_term->term.len = 0;
    term_dict->prev_term->doc_freq = 0;
    term_dict->prev_term->offset = 0;

    
    term_dict->cur_term = (sse_index_file_term_t *)sse_pool_alloc(pool, sizeof(sse_index_file_term_t));
    term_dict->cur_term->term.data = (char *)sse_pool_alloc(pool, SSE_TOKENIZER_TOKEN_BUF_SIZE);
    term_dict->cur_term->term.len = 0;
    term_dict->cur_term->doc_freq = 0;
    term_dict->cur_term->offset = 0;

    term_dict->file = SSE_NULL;
    term_dict->file_path = SSE_NULL;

    term_dict->pool = pool;
    term_dict->log = log;

    return term_dict;
}

sse_index_file_term_dict_index_t *
sse_index_file_term_dict_index_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_term_dict_index_t *term_dict_index;
    
    term_dict_index = (sse_index_file_term_dict_index_t *)sse_pool_calloc(pool, sizeof(sse_index_file_term_dict_index_t));
    if (!term_dict_index) {
        return SSE_NULL;
    }

    return term_dict_index;
}

sse_index_file_docid_freq_t *
sse_index_file_docid_freq_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_docid_freq_t *docid_freq;
    
    docid_freq = (sse_index_file_docid_freq_t *)sse_pool_calloc(pool, sizeof(sse_index_file_docid_freq_t));
    if (!docid_freq) {
        return SSE_NULL;
    }

    return docid_freq;
}

sse_index_file_offset_t *
sse_index_file_offset_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_offset_t *offset_file;
    
    offset_file = (sse_index_file_offset_t *)sse_pool_calloc(pool, sizeof(sse_index_file_offset_t));
    if (!offset_file) {
        return SSE_NULL;
    }

    return offset_file;
}

sse_index_file_doc_index_t *
sse_index_file_doc_index_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_doc_index_t *doc_index;
    
    doc_index = (sse_index_file_doc_index_t *)sse_pool_calloc(pool, sizeof(sse_index_file_doc_index_t));
    if (!doc_index) {
        return SSE_NULL;
    }

    doc_index->doc_count = 0;

    return doc_index;
}

sse_index_file_doc_t *
sse_index_file_doc_init(sse_pool_t *pool, sse_log_t *log)
{
    sse_index_file_doc_t *doc;
    
    doc = (sse_index_file_doc_t *)sse_pool_calloc(pool, sizeof(sse_index_file_doc_t));
    if (!doc) {
        return SSE_NULL;
    }

    return doc;
}

void
sse_index_file_term_dict_open(sse_index_file_term_dict_t *term_dict, const char *path, const char *type)
{
    term_dict->file_path = path;
    term_dict->file = fopen(term_dict->file_path, type);

    term_dict->term_num = 0;
    term_dict->term_count = sse_stdio_read_uint(term_dict->file);
    term_dict->term_block_size = sse_stdio_read_vuint(term_dict->file);
}

void
sse_index_file_term_dict_index_open(sse_index_file_term_dict_index_t *term_dict_index, const char *path, const char *type)
{
    term_dict_index->file_path = path;
    term_dict_index->file = fopen(term_dict_index->file_path, type);

    term_dict_index->index_count = sse_stdio_read_uint(term_dict_index->file);
    term_dict_index->base_offset = ftello(term_dict_index->file);

}

void
sse_index_file_docid_freq_open(sse_index_file_docid_freq_t *docid_freq, const char *path, const char *type)
{
    docid_freq->file_path = path;
    docid_freq->file = fopen(docid_freq->file_path, type);
}

void
sse_index_file_offset_open(sse_index_file_offset_t *offset_file, const char *path, const char *type)
{
    offset_file->file_path = path;
    offset_file->file = fopen(offset_file->file_path, type);
}

void
sse_index_file_doc_index_open(sse_index_file_doc_index_t *doc_index, const char *path, const char *type)
{
    doc_index->file_path = path;
    doc_index->file = fopen(doc_index->file_path, type);

    doc_index->doc_count = sse_stdio_read_uint(doc_index->file);
    doc_index->base_offset = ftello(doc_index->file);
}

void
sse_index_file_doc_open(sse_index_file_doc_t *doc, const char *path, const char *type)
{
    doc->file_path = path;
    doc->file = fopen(doc->file_path, type);
}

void
sse_index_file_term_dict_close(sse_index_file_term_dict_t *term_dict)
{
    fclose(term_dict->file);
}

void
sse_index_file_term_dict_index_close(sse_index_file_term_dict_index_t *term_dict_index)
{
    fclose(term_dict_index->file);
}

void
sse_index_file_docid_freq_close(sse_index_file_docid_freq_t *docid_freq)
{
    fclose(docid_freq->file);
}

void
sse_index_file_offset_close(sse_index_file_offset_t *offset_file)
{
    fclose(offset_file->file);
}

void
sse_index_file_doc_index_close(sse_index_file_doc_index_t *doc_index)
{
    fclose(doc_index->file);
}

void
sse_index_file_doc_close(sse_index_file_doc_t *doc)
{
    fclose(doc->file);
}

sse_index_file_term_t *
sse_index_file_term_dict_read(sse_index_file_term_dict_t *term_dict)
{
    sse_index_file_term_t       *temp, *cur_term, *prev_term;
    size_t                      prefix_len, suffix_len;

    if (term_dict->term_num == term_dict->term_count) {
        return SSE_NULL;
    }

    /* swap prev and cur term */
    temp = term_dict->prev_term;
    term_dict->prev_term = term_dict->cur_term;
    term_dict->cur_term = temp;

    cur_term = term_dict->cur_term;
    prev_term = term_dict->prev_term;

    /* read head term in the block */
    if (term_dict->term_num % term_dict->term_block_size == 0) {
        /* [string length][string] */
        cur_term->term.len = sse_stdio_read_vuint(term_dict->file);
        sse_stdio_read(cur_term->term.data, cur_term->term.len, term_dict->file);
        /* [doc frequency] */
        cur_term->doc_freq = sse_stdio_read_vuint(term_dict->file);
        /* [offset] in docid-freq index file */
        cur_term->offset = sse_stdio_read_vuint(term_dict->file);
    }
    /* read other term in the block */
    else {
        /* [prefix length][suffix length] */
        prefix_len = sse_stdio_read_vuint(term_dict->file);
        suffix_len = sse_stdio_read_vuint(term_dict->file);
        cur_term->term.len = prefix_len + suffix_len;
        if (prefix_len > 0) {
            sse_memcpy(cur_term->term.data, prev_term->term.data, prefix_len);
        }
        /* [suffix] */
        sse_stdio_read(cur_term->term.data + prefix_len, suffix_len, term_dict->file);
        /* [doc frequency] */
        cur_term->doc_freq = sse_stdio_read_vuint(term_dict->file);
        /* [offset] in docid-freq index file */
        cur_term->offset = prev_term->offset + sse_stdio_read_vuint(term_dict->file);
    }

    ++term_dict->term_num;
    return cur_term;
}

sse_index_file_term_t *
sse_index_file_term_dict_random_read(sse_index_file_term_dict_t *term_dict, sse_index_file_term_dict_index_t *term_dict_index, sse_uint_t term_num)
{
    sse_uint_t                  block_num, step;
    sse_off_t                   term_block_offset;
    sse_index_file_term_t       *cur_term, *prev_term, *temp;
    size_t                      prefix_len, suffix_len;

    if (term_num >= term_dict->term_count) {
        return SSE_NULL;
    }

    block_num = term_num / term_dict->term_block_size;
    step = term_num % term_dict->term_block_size;

    fseeko(term_dict_index->file, term_dict_index->base_offset + block_num * sizeof(sse_uint_t), SEEK_SET);
    term_block_offset = sse_stdio_read_uint(term_dict_index->file);

    /* read the first term in the block */
    cur_term = term_dict->cur_term;
    prev_term = term_dict->prev_term;
    
    fseeko(term_dict->file, term_block_offset, SEEK_SET);
    /* [string length][string] */
    cur_term->term.len = sse_stdio_read_vuint(term_dict->file);
    sse_stdio_read(cur_term->term.data, cur_term->term.len, term_dict->file);
    /* [doc frequency] */
    cur_term->doc_freq = sse_stdio_read_vuint(term_dict->file);
    /* [offset] in docid-freq index file */
    cur_term->offset = sse_stdio_read_vuint(term_dict->file);

    while (step > 0) {
        --step;
        /* swap prev and cur term */
        temp = prev_term;
        prev_term = cur_term;
        cur_term = temp;

        /* [prefix length][suffix length] */
        prefix_len = sse_stdio_read_vuint(term_dict->file);
        suffix_len = sse_stdio_read_vuint(term_dict->file);
        cur_term->term.len = prefix_len + suffix_len;
        if (prefix_len > 0) {
            sse_memcpy(cur_term->term.data, prev_term->term.data, prefix_len);
        }
        /* [suffix] */
        sse_stdio_read(cur_term->term.data + prefix_len, suffix_len, term_dict->file);
        /* [doc frequency] */
        cur_term->doc_freq = sse_stdio_read_vuint(term_dict->file);
        /* [offset] in docid-freq index file */
        cur_term->offset = prev_term->offset + sse_stdio_read_vuint(term_dict->file);
    }

    term_dict->term_num = term_num;
    return cur_term;
}

void
sse_index_file_docid_freq_read(sse_index_file_docid_freq_t *docid_freq, sse_off_t begin_offset, sse_freq_t doc_freq, sse_vector_t *docid_freq_list)
{
    sse_index_file_posting_t        posting;
    sse_uint_t                      i;

    fseeko(docid_freq->file, begin_offset, SEEK_SET);

    /* read the first posint info */
    posting.doc_id = sse_stdio_read_vuint(docid_freq->file);
    posting.term_freq = sse_stdio_read_vuint(docid_freq->file);
    posting.weight = sse_stdio_read_float(docid_freq->file);
    posting.offset = sse_stdio_read_vuint(docid_freq->file);
    sse_vector_push_back(docid_freq_list, &posting);

    for (i = 1; i < doc_freq; ++i) {
        posting.doc_id = posting.doc_id + sse_stdio_read_vuint(docid_freq->file);
        posting.term_freq = sse_stdio_read_vuint(docid_freq->file);
        posting.weight = sse_stdio_read_float(docid_freq->file);
        posting.offset = posting.offset + sse_stdio_read_vuint(docid_freq->file);
        sse_vector_push_back(docid_freq_list, &posting);
    }
}

void
sse_index_file_offset_read(sse_index_file_offset_t *offset_file, sse_off_t begin_offset, sse_freq_t term_freq, sse_vector_t *offset_list)
{
    //sse_index_file_offset_data_t      offset_data;
    sse_off_t                           start;
    sse_uint_t                          i;

    fseeko(offset_file->file, begin_offset, SEEK_SET);

    /* read the first posint info */
    start = sse_stdio_read_vuint(offset_file->file);
    sse_stdio_read_vuint(offset_file->file);
    sse_vector_push_back(offset_list, &start);

    for (i = 1; i < term_freq; ++i) {
        start = start + sse_stdio_read_vuint(offset_file->file);
        sse_stdio_read_vuint(offset_file->file);
        sse_vector_push_back(offset_list, &start);
    }
}

char *
sse_index_file_doc_name_read(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_pool_t *pool, sse_uint_t *doc_size)
{
    sse_off_t   doc_offset;
    sse_uint_t  doc_name_size;
    char *      doc_name;

    fseeko(doc_index->file, doc_index->base_offset + (doc_id - 1) * sizeof(sse_uint_t), SEEK_SET);
    doc_offset = sse_stdio_read_uint(doc_index->file);

    fseeko(doc->file, doc_offset, SEEK_SET);
    doc_name_size = sse_stdio_read_uint(doc->file);
    *doc_size = sse_stdio_read_uint(doc->file);

    doc_name = (char *)sse_pool_calloc(pool, doc_name_size + 1);
    sse_stdio_read(doc_name, doc_name_size, doc->file);

    return doc_name;
}

size_t
sse_index_file_doc_read(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_off_t begin_offset, void *buf, size_t size)
{
    sse_off_t   doc_offset;
    sse_uint_t  doc_name_size, doc_size;

    fseeko(doc_index->file, doc_index->base_offset + (doc_id - 1) * sizeof(sse_uint_t), SEEK_SET);
    doc_offset = sse_stdio_read_uint(doc_index->file);

    fseeko(doc->file, doc_offset, SEEK_SET);
    doc_name_size = sse_stdio_read_uint(doc->file);
    doc_size = sse_stdio_read_uint(doc->file);
    
    fseeko(doc->file, doc_name_size + begin_offset, SEEK_CUR);
    
    if (begin_offset + size > doc_size) {
        size = doc_size - begin_offset;
    }

    sse_stdio_read(buf, size, doc->file);

    return size;
}

char *
sse_index_file_doc_get(sse_index_file_doc_index_t *doc_index, sse_index_file_doc_t *doc, sse_id_t doc_id, sse_pool_t *pool)
{
    sse_off_t   doc_offset;
    sse_uint_t  doc_name_size, doc_size;
    char        *buf;

    fseeko(doc_index->file, doc_index->base_offset + (doc_id - 1) * sizeof(sse_uint_t), SEEK_SET);
    doc_offset = sse_stdio_read_uint(doc_index->file);

    fseeko(doc->file, doc_offset, SEEK_SET);
    doc_name_size = sse_stdio_read_uint(doc->file);
    doc_size = sse_stdio_read_uint(doc->file);
    
    fseeko(doc->file, doc_name_size, SEEK_CUR);

    buf = (char *)sse_pool_calloc(pool, doc_size + 1);
    sse_stdio_read(buf, doc_size, doc->file);

    return buf;
}
