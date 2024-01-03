#ifndef _SSE_LOG_H_INCLUDED_
#define _SSE_LOG_H_INCLUDED_

#include <sse_core.h>

typedef struct {
	sse_uint_t		log_level;
	FILE			*log_file;
	char			*log_path;
} sse_log_t;


sse_log_t *sse_log_init(const char *log_dir, const char *log_name, const sse_uint_t log_level);
void sse_log_close(sse_log_t *log);
void sse_log(sse_uint_t level, sse_log_t *log, sse_bool_t errno_flag, const char *fmt, ...);
void sse_log_core(sse_uint_t level, sse_log_t *log, sse_bool_t errno_flag, int error, const char *fmt, va_list args);

#endif /* _SSE_LOG_H_INCLUDED_ */
