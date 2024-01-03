#include <sse_core.h>
#include <sse_index.h>

static void sse_tokenizer_token_fetch(sse_tokenizer_t *tokenizer, sse_rbtree_t *term_dict);
static void sse_tokenizer_token_buf_push(sse_tokenizer_t *tokenizer, char c);
static void sse_tokenizer_split_words(sse_tokenizer_t *tokenizer, const char *doc_path, sse_rbtree_t *term_dict, sse_off_t doc_size);
static void sse_tokenizer_token_tolower(sse_str_t *token);
static void sse_tokenizer_stopwords_filter_init(sse_tokenizer_t *tokenizer);
static void sse_tokenizer_save_doc(FILE *doc_data_index_file, FILE *doc_data_file, char *doc_name, const char *doc_path, sse_off_t doc_size, char *buf, size_t buf_size, sse_tokenizer_t *tokenizer);

sse_tokenizer_t *
sse_tokenizer_create(sse_index_build_counter_t *counter, sse_pool_t *pool, sse_log_t *log)
{
    sse_tokenizer_t *tokenizer;

    tokenizer = (sse_tokenizer_t *)sse_pool_alloc(pool, sizeof(sse_tokenizer_t));
    if (!tokenizer) {
        return SSE_NULL;
    }

    tokenizer->pool = pool;
    tokenizer->log = log;
    
    tokenizer->token_buf = (sse_token_buf_t *)sse_pool_alloc(pool, sizeof(sse_token_buf_t));
    tokenizer->token_buf->buf = (char *)sse_pool_alloc(pool, SSE_TOKENIZER_TOKEN_BUF_SIZE);
    tokenizer->token_buf->size = SSE_TOKENIZER_TOKEN_BUF_SIZE;
    SSE_TOKEN_BUF_EMPTY(tokenizer->token_buf);

    tokenizer->token = (sse_str_t *)sse_pool_alloc(pool, sizeof(sse_str_t));
    sse_str_set_null(tokenizer->token);
    tokenizer->token->data = (char *)sse_pool_alloc(pool, SSE_TOKENIZER_TOKEN_BUF_SIZE);

    /* init stop words hash table */
    sse_tokenizer_stopwords_filter_init(tokenizer);

    tokenizer->cur_doc_id = 0;
    tokenizer->cur_offset = 0;

    tokenizer->counter = counter;
    tokenizer->counter->index_file_count = 0;

    return tokenizer;
}

int
sse_tokenizer_execute(sse_tokenizer_t *tokenizer)
{
    DIR                 *doc_dir;
    struct dirent       *dirp;
    char                *path_buf, *doc_data_index_path, *doc_data_path; 
    const char          *doc_dir_path; 
    char                *doc_path;
    size_t              doc_dir_len;
    struct stat         statbuf;
    size_t              cumulative_size = 0;
    sse_pool_t          *term_dict_pool = SSE_NULL;
    sse_rbtree_t        *term_dict;
    FILE                *doc_data_index_file, *doc_data_file;
    char                *copy_buf;
    sse_off_t           doc_total_size = 0;     

    doc_dir_path = CONFIG.DOC_DIR;
    if ((doc_dir = opendir(doc_dir_path)) == SSE_NULL) {
        sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_FALSE, "Cannot open dir: %s.", doc_dir_path);
        return SSE_ERROR;
}

    /* clean up and create the temp and index directory */
    sse_dir_cleanup(CONFIG.TEMP_DIR);
    sse_dir_create(CONFIG.TEMP_DIR);
    sse_dir_cleanup(CONFIG.INDEX_DIR);
    sse_dir_create(CONFIG.INDEX_DIR);

    /* directory path */
    path_buf = (char *)sse_pool_alloc(tokenizer->pool, SSE_TOKENIZER_PATH_SIZE);
    doc_dir_len = sse_strlen(doc_dir_path);
    sse_strncpy(path_buf, doc_dir_path, doc_dir_len);
    if (path_buf[doc_dir_len - 1] != '/') {
        path_buf[doc_dir_len++] = '/';
    }

    /* open doc data and doc data index file */
    doc_data_index_path = (char *)sse_pool_alloc(tokenizer->pool, SSE_PATH_MAX_SIZE);
    doc_data_path = (char *)sse_pool_alloc(tokenizer->pool, SSE_PATH_MAX_SIZE);
    sse_file_path_fill(doc_data_index_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);sse_file_path_fill(doc_data_index_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION);
    sse_file_path_fill(doc_data_path, CONFIG.INDEX_DIR, CONFIG.INDEX_NAME, CONFIG.INDEX_DOC_DATA_FILE_EXTENSION);

    doc_data_index_file = fopen(doc_data_index_path, "w");
    doc_data_file = fopen(doc_data_path, "w");
    sse_stdio_write_uint(0, doc_data_index_file);

    copy_buf = sse_pool_alloc(tokenizer->pool, SSE_FILE_COPY_BUF_SIZE);

    while ((dirp = readdir(doc_dir)) != SSE_NULL) {
        sse_strcpy(path_buf + doc_dir_len, dirp->d_name);
        doc_path = path_buf;

        /* get the info of doc */
        if (stat(doc_path, &statbuf) < 0) {
            sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_FALSE, "Failure: stat[%s].", doc_path);
            continue;
        }
        /* whether the type of the file is regular */
        if (!S_ISREG(statbuf.st_mode)) 
        {
            continue;
        }

        if (term_dict_pool == SSE_NULL) {
            term_dict_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, tokenizer->log);
            term_dict = sse_term_dict_create(term_dict_pool, tokenizer->log);
        }
        sse_tokenizer_split_words(tokenizer, doc_path, term_dict, statbuf.st_size);

        cumulative_size += statbuf.st_size;
        if (cumulative_size >= CONFIG.TOKENIZER_FILE_MAX_AUMULATIVE_SIZE) {
            /* save term dictionary to index files */
            sse_term_dict_save(term_dict, CONFIG.TEMP_DIR, tokenizer->counter);
            /* sse_term_dict_debug(term_dict); */
            printf("last doc id: %d\n", tokenizer->cur_doc_id);
            
            /* free memory related with term dictionary */
            sse_pool_destory(term_dict_pool);
            term_dict_pool = SSE_NULL;

            /* clean up cumulative size */
            cumulative_size = 0;
        }

        /* save doc data */
        sse_tokenizer_save_doc(doc_data_index_file, doc_data_file, dirp->d_name, doc_path, statbuf.st_size, copy_buf, SSE_FILE_COPY_BUF_SIZE, tokenizer);
        doc_total_size += statbuf.st_size;
    }
    
    if (term_dict_pool) {
        sse_term_dict_save(term_dict, CONFIG.TEMP_DIR, tokenizer->counter);
        /* sse_term_dict_debug(term_dict); */
        sse_pool_destory(term_dict_pool);
    }
    /* correct the doc count */
    fseeko(doc_data_index_file, 0, SEEK_SET);
    sse_stdio_write_uint(tokenizer->cur_doc_id, doc_data_index_file);
    closedir(doc_dir);

    fclose(doc_data_index_file);
    fclose(doc_data_file);

    sse_log(SSE_LOG_INFO, tokenizer->log, SSE_BOOL_FALSE, "total doc size: %d", doc_total_size);
    
    return SSE_OK;
}

static void
sse_tokenizer_save_doc(FILE *doc_data_index_file, FILE *doc_data_file, char *doc_name, const char *doc_path, sse_off_t doc_size, char *buf, size_t buf_size, sse_tokenizer_t *tokenizer) 
{
    FILE            *doc;
    sse_off_t       offset, begin_offset, end_offset;
    size_t          doc_name_size;

    doc_name_size = sse_strlen(doc_name);

    doc = fopen(doc_path, "r");
    
    offset = ftello(doc_data_file);
    sse_stdio_write_uint(offset, doc_data_index_file);

    sse_stdio_write_uint(doc_name_size, doc_data_file);
    sse_stdio_write_uint(doc_size, doc_data_file);
    sse_stdio_write(doc_name, doc_name_size, doc_data_file);

    begin_offset = ftello(doc_data_file);
    while (fgets(buf, buf_size, doc) != SSE_NULL) {
        if (fputs(buf, doc_data_file) == EOF) {
            exit(1);
        }
    }
    end_offset = ftello(doc_data_file);
    if ((end_offset - begin_offset) != doc_size)
    {
        sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_FALSE, "doc size unmatch: %s, %d-%d.", doc_path, end_offset-begin_offset, doc_size);
    }
    fclose(doc);
}

static void
sse_tokenizer_split_words(sse_tokenizer_t *tokenizer, const char *doc_path, sse_rbtree_t *term_dict, sse_off_t doc_size)
{
    FILE                *doc;
    int                 c;
    sse_token_buf_t     *token_buf;
    sse_str_t           *token;
    sse_off_t           doc_size_check = 0;

    doc = fopen(doc_path, "r");
    if (doc == SSE_NULL) {
        sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_TRUE, "File open failure: %s.", doc_path);
        return;
    }
    token_buf = tokenizer->token_buf;
    token = tokenizer->token;
    SSE_TOKEN_BUF_EMPTY(token_buf);
    
    ++tokenizer->cur_doc_id;
    tokenizer->cur_offset = -1;

    while ((c = getc(doc)) != EOF) {
        ++doc_size_check;
        ++tokenizer->cur_offset;
        if (isprint(c)) {
            if (isalpha(c)) {   /* a-z A-Z */
                if (SSE_TOKEN_BUF_FLAGS_HAVE_COMMA(token_buf)
                            || SSE_TOKEN_BUF_FLAGS_HAVE_PERIOD(token_buf)) {
                    sse_tokenizer_token_fetch(tokenizer, term_dict);
                }
                sse_tokenizer_token_buf_push(tokenizer, c);
                SSE_TOKEN_BUF_FLAGS_SET_ALPHA(token_buf);
            }
            else if (isdigit(c)) { /* 0-9 */
                sse_tokenizer_token_buf_push(tokenizer, c);
                SSE_TOKEN_BUF_FLAGS_SET_DIGIT(token_buf);
            }
            else if (c == '-') {
                if (SSE_TOKEN_BUF_FLAGS_HAVE_ALPHA(token_buf)
                            && !SSE_TOKEN_BUF_FLAGS_HAVE_DIGIT(token_buf)) {
                    sse_tokenizer_token_buf_push(tokenizer, c);
                }
                else {
                    sse_tokenizer_token_fetch(tokenizer, term_dict);
                }
            }
            else if (c == '$') {
                if (SSE_TOKEN_BUF_IS_EMPTY(token_buf)) {
                    sse_tokenizer_token_buf_push(tokenizer, c);
                }
                else {
                    sse_tokenizer_token_fetch(tokenizer, term_dict);
                }
            }
            else if (c == '%') {
                if (!SSE_TOKEN_BUF_FLAGS_HAVE_ALPHA(token_buf)
                            && SSE_TOKEN_BUF_FLAGS_HAVE_DIGIT(token_buf)) {
                    sse_tokenizer_token_buf_push(tokenizer, c);
                }
                sse_tokenizer_token_fetch(tokenizer, term_dict);
            }
            else if (c == ',' || c == '.') {    
                if (!SSE_TOKEN_BUF_FLAGS_HAVE_ALPHA(token_buf)
                            && SSE_TOKEN_BUF_FLAGS_HAVE_DIGIT(token_buf)) {
                    sse_tokenizer_token_buf_push(tokenizer, c);
                    if (c == ',') {
                        SSE_TOKEN_BUF_FLAGS_SET_COMMA(token_buf);
                    }
                    else {
                        SSE_TOKEN_BUF_FLAGS_SET_PERIOD(token_buf);
                    }
                }
                else {
                    sse_tokenizer_token_fetch(tokenizer, term_dict);
                }

            }
            else if (isspace(c)) {
                sse_tokenizer_token_fetch(tokenizer, term_dict);
            }
            
            else {
                sse_tokenizer_token_fetch(tokenizer, term_dict);
            }
        }
        else {
            if (isspace(c)) {
                sse_tokenizer_token_fetch(tokenizer, term_dict);
            }
        }
    } /* end while */

    sse_tokenizer_token_fetch(tokenizer, term_dict);

    fclose(doc);

    if(doc_size_check != doc_size) {
        sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_FALSE, "doc size unmatch(split): %s, %d-%d.", doc_path, doc_size_check, doc_size);
    }
}

static void
sse_tokenizer_token_fetch(sse_tokenizer_t *tokenizer, sse_rbtree_t *term_dict)
{
    size_t              len;
    char                tail;
    sse_token_buf_t     *token_buf;
    sse_str_t           *token;

    token_buf = tokenizer->token_buf;
    token = tokenizer->token;

    if (!SSE_TOKEN_BUF_IS_EMPTY(token_buf)) {
        if ((tail = token_buf->buf[token_buf->pos - 1]) == ',' || tail == '.' || tail == '-' || tail == '$') {
            len = token_buf->pos - 1;
        }
        else {
            len = token_buf->pos;
        }
        if (len == 0) {
            SSE_TOKEN_BUF_EMPTY(token_buf);
            return;
        }
        sse_memcpy(token->data, token_buf->buf, len);
        token->len = len;

        /* lower the token */
        sse_tokenizer_token_tolower(token);
        /* filter stop words */
        if (!sse_hash_contains(tokenizer->stopwords_dict, token)) {
            /* stemming */
            token->len = stem(token->data, 0, token->len - 1) + 1;
            /* add into term dictionary */
            sse_term_dict_add(term_dict, token, token_buf->offset, len, tokenizer->cur_doc_id);
        }

        /* clean up the token buffer */
        SSE_TOKEN_BUF_EMPTY(token_buf);
    }
}

static void 
sse_tokenizer_token_buf_push(sse_tokenizer_t *tokenizer, char c)
{
    if (SSE_TOKEN_BUF_IS_FULL(tokenizer->token_buf)) {
        sse_log(SSE_LOG_FATAL, tokenizer->log, SSE_BOOL_FALSE, "Buffer overflow: token_buf[%d].", tokenizer->token_buf->size);
        exit(1);
    }

    if (SSE_TOKEN_BUF_IS_EMPTY(tokenizer->token_buf)) {
        tokenizer->token_buf->offset = tokenizer->cur_offset;
    }
    tokenizer->token_buf->buf[tokenizer->token_buf->pos++] = c;
}

static void
sse_tokenizer_token_tolower(sse_str_t *token)
{
    size_t len = token->len;
    char *c;

    c = token->data;
    while (len) {
        if (*c >= 'A' && *c <= 'Z') {
            *c = *c | 0x20;
        }
        ++c;
        --len;
    }
}

static void
sse_tokenizer_stopwords_filter_init(sse_tokenizer_t *tokenizer)
{
    FILE        *sw_fp;
    char        *buf;
    size_t      buf_size, len;
    sse_str_t   *stopword;
    sse_hash_t  *stopwords_dict;

    sw_fp = fopen(CONFIG.STOPWORDS_FILE, "r");
    buf_size = SSE_TOKENIZER_STOPWORD_MAX_SIZE;
    buf = (char *)sse_pool_alloc(tokenizer->pool, buf_size);

    if (sw_fp == SSE_NULL) {
        sse_log(SSE_LOG_ERROR, tokenizer->log, SSE_BOOL_TRUE, "File open failure: %s.", CONFIG.STOPWORDS_FILE);
        return;
    }
    
    tokenizer->stopwords_dict = sse_hash_create(SSE_TOKENIZER_STOPWORDS_HASHTABLE_SIZE, tokenizer->pool, tokenizer->log);
    stopwords_dict = tokenizer->stopwords_dict;

    while (fgets(buf, buf_size, sw_fp) != SSE_NULL) {
        /* create a new stopword of sse_str_t obj */
        stopword = (sse_str_t *)sse_pool_alloc(tokenizer->pool, sizeof(sse_str_t));
        len = sse_strlen(buf);

        /* omit the LF at the end of string */
        if (buf[len - 1] == '\n') {
            --len;
        }
        stopword->len = len;
        stopword->data = (char *)sse_pool_alloc(tokenizer->pool, stopword->len);
        sse_memcpy(stopword->data, buf, stopword->len);

        sse_hash_add(stopwords_dict, stopword, SSE_NULL);
    }

    fclose(sw_fp);
}
