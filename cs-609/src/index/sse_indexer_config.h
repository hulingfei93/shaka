#ifndef _SSE_INDEXER_CONFIG_H_INCLUDED_
#define _SSE_INDEXER_CONFIG_H_INCLUDED_

#include <sse_core.h>
#include <sse_index.h>

typedef struct {
    char            MODE;
    const char      *LOG_DIR;
    const char      *LOG_NAME;
    sse_uint_t      LOG_LEVEL;
    
    const char      *DOC_DIR;
    const char      *INDEX_DIR;
    const char      *TEMP_DIR;
    const char      *STOPWORDS_FILE;
    
    size_t          TOKENIZER_FILE_MAX_AUMULATIVE_SIZE;     

    const char      *INDEX_TERM_DICT_FILE_EXTENSION;
    sse_uint_t      INDEX_TERM_DICT_FILE_BLOCK_SIZE;
    const char      *INDEX_TERM_DICT_INDEX_FILE_EXTENSION;

    const char      *INDEX_DOCID_FREQ_FILE_EXTENSION;
    
    const char      *INDEX_OFFSET_FILE_EXTENSION;

    const char      *INDEX_DOC_DATA_INDEX_FILE_EXTENSION;
    const char      *INDEX_DOC_DATA_FILE_EXTENSION;

    const char      *INDEX_NAME;

} sse_indexer_config_t;

extern sse_indexer_config_t CONFIG;

void sse_indexer_config_init(const char *doc_dir, const char *index_dir, const char *temp_dir, const char *stopwords_file);

#endif /* _SSE_INDEXER_CONFIG_H_INCLUDED_ */
