#ifndef _SSE_SERVER_CONFIG_H_INCLUDED_
#define _SSE_SERVER_CONFIG_H_INCLUDED_

#include <sse_core.h>
#include <sse_server.h>

typedef struct {
	char			MODE;
	const char		*LOG_DIR;
	const char		*LOG_NAME;
	sse_uint_t		LOG_LEVEL;
	
	const char		*INDEX_DIR;
	const char		*STOPWORDS_FILE;

	const char		*INDEX_TERM_DICT_FILE_EXTENSION;
	const char		*INDEX_TERM_DICT_INDEX_FILE_EXTENSION;

	const char		*INDEX_DOCID_FREQ_FILE_EXTENSION;
	
	const char		*INDEX_OFFSET_FILE_EXTENSION;

	const char		*INDEX_DOC_DATA_INDEX_FILE_EXTENSION;
	const char		*INDEX_DOC_DATA_FILE_EXTENSION;

	const char		*INDEX_NAME;

} sse_server_config_t;

extern sse_server_config_t SERVER_CONFIG;

void sse_server_config_init(const char *index_dir, const char *stopwords_file);

#endif /* _SSE_SERVER_CONFIG_H_INCLUDED_ */
