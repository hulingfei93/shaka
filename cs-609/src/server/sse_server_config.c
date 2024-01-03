#include <sse_core.h>
#include <sse_server.h>


sse_server_config_t SERVER_CONFIG;

void
sse_server_config_init(const char *index_dir, const char *stopwords_file)
{
    SERVER_CONFIG.MODE = SSE_MODE_NORMAL;
    SERVER_CONFIG.LOG_DIR = "/var/log/sse";
    SERVER_CONFIG.LOG_NAME = "server.log";
    SERVER_CONFIG.LOG_LEVEL = SSE_LOG_INFO;

    SERVER_CONFIG.INDEX_DIR = index_dir;
    SERVER_CONFIG.STOPWORDS_FILE = stopwords_file;

    SERVER_CONFIG.INDEX_TERM_DICT_FILE_EXTENSION = ".td";
    SERVER_CONFIG.INDEX_TERM_DICT_INDEX_FILE_EXTENSION = ".tdi";
    
    SERVER_CONFIG.INDEX_DOCID_FREQ_FILE_EXTENSION = ".df";
    
    SERVER_CONFIG.INDEX_OFFSET_FILE_EXTENSION = ".ofs";

    SERVER_CONFIG.INDEX_DOC_DATA_INDEX_FILE_EXTENSION = ".ddi";
    SERVER_CONFIG.INDEX_DOC_DATA_FILE_EXTENSION = ".dd";

    SERVER_CONFIG.INDEX_NAME = "index";
}


