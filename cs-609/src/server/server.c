#include <sse_core.h>
#include <sse_server.h>

#include <onion/onion.h>
#include <onion/log.h>
#include <signal.h>
#include <netdb.h>

#define SSE_SEARCH_RESULTS_MIN_TOP		20
#define SSE_SEARCH_RESULTS_MAX_TOP		100

sse_log_t		*server_log;
sse_server_t	*server;
sse_pool_t		*server_pool;
onion			*o = SSE_NULL;

void usage()
{
	printf("Usage: server <indexdir> <stopwords_file> <query string> [-d].\n");
}

static void shutdown_server(int _)
{
	if (o) {
		onion_listen_stop(o);
	}
	sse_pool_debug_info(server_pool);
	sse_pool_destory(server_pool);
}

int home(void *p, onion_request *request, onion_response *response)
{
	onion_response_write0(response, "<form action='search' style='width:700px;margin:300px auto;text-aligh:center'>");
	onion_response_write0(response, "<input type='text' name='q' style='width:500px'/>");
	onion_response_write0(response, "<input type='submit' value='Search' style='margin-left:30px'/>");
	onion_response_write0(response, "</form>");
	return OCS_PROCESSED;
}

int doc(void *p, onion_request *request, onion_response *response)
{
	sse_pool_t		*query_pool;
	sse_uint_t		doc_id;
	const char		*doc_id_str;
	char			 *doc_content;

	query_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, server_log);
	doc_id_str = onion_request_get_query(request, "id");

	if (doc_id_str == SSE_NULL || sse_strlen(doc_id_str) == 0) {
		return OCS_NOT_PROCESSED;
	}
	doc_id = atoi(doc_id_str);
	doc_content = sse_server_search_doc(doc_id, server, query_pool, server_log);
	
	if (doc_content == SSE_NULL) {
		return OCS_NOT_PROCESSED;
	}

	onion_response_write0(response, "<div>");
	onion_response_write0(response, doc_content);
	onion_response_write0(response, "</div>");
	
	/* release resources */
	sse_pool_destory(query_pool);

	return OCS_PROCESSED;
	
}

int search(void *p, onion_request *request, onion_response *response)
{
	sse_pool_t						*query_pool;
	char							*query_text;
	const char						*query_string;
	const char						*model_string;
	sse_server_search_model_t		search_model;
	const char						*top_string;
	sse_uint_t						top;
	size_t							query_len, i;
	sse_bool_t						has_result;
	sse_server_search_query_t *		query;
	sse_server_search_result_t		*result;

	query_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, server_log);
	query_string = onion_request_get_query(request, "q");
	model_string = onion_request_get_query(request, "m");
	top_string = onion_request_get_query(request, "t");

	if (model_string == SSE_NULL || sse_strlen(model_string) == 0 || sse_strcmp(model_string, "2") != 0) {
		search_model = STANDARD_MODEL;
	}
	else {
	
		search_model = OMIT_DOCLEN_MODEL;
	}
	
	if (top_string == SSE_NULL || sse_strlen(top_string) == 0) {
		top = SSE_SEARCH_RESULTS_MIN_TOP;
	}
	else {
	
		top = atoi(top_string);
		if (top < SSE_SEARCH_RESULTS_MIN_TOP) {
			top = SSE_SEARCH_RESULTS_MIN_TOP;
		}
		else if (top > SSE_SEARCH_RESULTS_MAX_TOP) {
			top = SSE_SEARCH_RESULTS_MAX_TOP;
		}
	}

	if (query_string == SSE_NULL || sse_strlen(query_string) == 0) {
		has_result = SSE_BOOL_FALSE;
	}
	else {
		query_len = sse_strlen(query_string);
		query_text = sse_pool_alloc(query_pool, query_len + 1);
		sse_strcpy(query_text, query_string);
		query = sse_server_search(query_text, top, search_model, server, query_pool, server_log);

		if (query->results_len == 0) {
			has_result = SSE_BOOL_FALSE;
		}
		else {
			has_result = SSE_BOOL_TRUE;
		}
	}

	/*********generate html************/
	/* search form */
	onion_response_write0(response, "<form action='search' style='margin-left:50px'>");
	onion_response_printf(response, "<p style='color:FF0000'>%u Documents</p>", server->doc_count);
	if (query_string == SSE_NULL || sse_strlen(query_string) == 0) {
		onion_response_write0(response, "<input type='text' name='q' style='width:300px'/>");
	}
	else {
		onion_response_printf(response, "<input type='text' name='q' value='%s' style='width:300px'/>", query_string);
	}
	onion_response_write0(response, "<input type='submit' value='Search' style='margin-left:30px'/>");
	onion_response_write0(response, "</form>");

	/* search result */
	onion_response_write0(response, "<ul class='result-list'>");
 
	if (!has_result) {
		onion_response_write0(response, "<li>No Results</li>");
	}
	else {
		for (i = 0; i < query->results_len; ++i) {
			onion_response_write0(response, "<li>");
			result = query->results + i;
			onion_response_printf(response, "<h3 style='margin:20px 0 5px 0'><a href='/doc?id=%u' target='_blank'>%s (score: %f doc size: %u)</a></h3>", result->doc_id, result->title, result->score, result->doc_size);
			onion_response_printf(response, "<p style='margin:5px 0 10px 0'>%s</p>", result->summary);
			onion_response_write0(response, "</li>");
		}
	}

	onion_response_write0(response, "</ul>");

	/* release resources */
	sse_pool_destory(query_pool);

	return OCS_PROCESSED;
}

int 
main(int argc, char **argv)
{
	if (argc < 4) {
		usage();
		return 1;
	}

	sse_server_config_init(argv[1], argv[2]);
	if (argc > 4 && sse_strcmp(argv[4], "-d") == 0) {
		SERVER_CONFIG.MODE = SSE_MODE_DEBUG;
	}
	
	/* create log */
	if (SERVER_CONFIG.MODE == SSE_MODE_DEBUG) {
		server_log = sse_log_init(SSE_NULL, SSE_NULL, SERVER_CONFIG.LOG_LEVEL);
	}
	else {
		server_log = sse_log_init(SERVER_CONFIG.LOG_DIR, SERVER_CONFIG.LOG_NAME, SERVER_CONFIG.LOG_LEVEL);
	}

	/* create and init server */
	server_pool = sse_pool_create(SSE_POOL_LARGE_SIZE, server_log);
	server = sse_server_create(server_pool, server_log);
	sse_server_init(server);

	signal(SIGINT, shutdown_server);
	signal(SIGTERM, shutdown_server);

	o = onion_new(O_POOL);
	onion_set_timeout(o, 5000);
	onion_set_hostname(o, argv[3]);
	onion_url *urls = onion_root_url(o);

	onion_url_add(urls, "", home);
	onion_url_add(urls, "search", search);
	onion_url_add(urls, "doc", doc);

	onion_listen(o);
	onion_free(o);


#if 0
	/* create query */
	sse_pool_t *query_pool = sse_pool_create(SSE_POOL_DEFAULT_SIZE, log);
	const char *query_string = argv[3];
	size_t query_len = sse_strlen(query_string);
	char *query_text = sse_pool_alloc(query_pool, query_len + 1);
	sse_strcpy(query_text, query_string);
	sse_server_search(query_text, 10, server, query_pool, log);

	sse_pool_destory(query_pool);

	sse_pool_debug_info(server_pool);
	sse_pool_destory(server_pool);
#endif

	return 0;
}
