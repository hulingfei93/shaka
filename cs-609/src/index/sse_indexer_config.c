#include <sse_core.h>
#include <sse_index.h>


sse_indexer_config_t CONFIG;

void
sse_indexer_config_init(const char *doc_dir, const char *index_dir, const char *temp_dir, const char *stopwords_file)
{
	CONFIG.MODE = SSE_MODE_NORMAL;
	CONFIG.LOG_DIR = "/var/log/sse";
	CONFIG.LOG_NAME = "indexer.log";
	CONFIG.LOG_LEVEL = SSE_LOG_INFO;

	CONFIG.DOC_DIR = doc_dir;
	CONFIG.INDEX_DIR = index_dir;
	CONFIG.TEMP_DIR = temp_dir;
	CONFIG.STOPWORDS_FILE = stopwords_file;

	CONFIG.TOKENIZER_FILE_MAX_AUMULATIVE_SIZE = 10 * 1024 * 1024;

	CONFIG.INDEX_TERM_DICT_FILE_EXTENSION = ".td";
	CONFIG.INDEX_TERM_DICT_FILE_BLOCK_SIZE = 10;
	CONFIG.INDEX_TERM_DICT_INDEX_FILE_EXTENSION = ".tdi";
	
	CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION = ".df";
	
	CONFIG.INDEX_OFFSET_FILE_EXTENSION = ".ofs";

	CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION = ".ddi";
	CONFIG.INDEX_DOC_DATA_FILE_EXTENSION = ".dd";

	CONFIG.INDEX_NAME = "index";
}


