#include <sse_core.h>

static sse_log_t _log;
static char log_buf[SSE_LOG_BUF_DEFAULT_SIZE];

sse_log_t *
sse_log_init(const char *log_dir, const char *log_name, const sse_uint_t log_level)
{
	size_t log_dir_len, log_name_len, log_path_len, len;
	_log.log_level = log_level;
	if (log_dir == SSE_NULL) {
		_log.log_file = SSE_STDIO_STDOUT;
		_log.log_path = SSE_NULL;
	}
	else
	{
		umask(0);
		if (opendir(log_dir) == SSE_NULL) {
			mkdir(log_dir, SSE_DIR_DEFAULT_MODE);	
		}
	
		log_dir_len = sse_strlen(log_dir);
		log_name_len = sse_strlen(log_name);

		if (log_dir[log_dir_len - 1] == '/') {
			log_path_len = log_dir_len + log_name_len;
			_log.log_path = (char *) malloc(log_path_len + 1);
			sse_strcpy(_log.log_path, log_dir);
			sse_strcpy(_log.log_path + log_dir_len, log_name);
		} 
		else {
			log_path_len = log_dir_len + log_name_len + 1;
			_log.log_path = (char *) malloc(log_path_len + 1);
			sse_strcpy(_log.log_path, log_dir);
			_log.log_path[log_dir_len] = '/';
			sse_strcpy(_log.log_path + log_dir_len + 1, log_name);
		}

		_log.log_file = fopen(_log.log_path, "a");

		if (_log.log_file == SSE_NULL) {
			len = sprintf(log_buf, "[alert] could not create log at %s.\n", _log.log_path);
			write(SSE_FD_STDERR, log_buf, len);
	        exit (1);
		}
	}

	return &_log;
}

void
sse_log_close(sse_log_t *log)
{
	if (log->log_file != SSE_STDIO_STDOUT) {
		fclose(log->log_file);
		free(log->log_path);
	}
}

void
sse_log(sse_uint_t level, sse_log_t *log, sse_bool_t errno_flag, const char *fmt, ...)
{
	if (log->log_level >= level) {
		va_list args;
		va_start(args, fmt);
		sse_log_core(level, log, errno_flag, errno_flag ? errno : 0, fmt, args);
		va_end(args);
	}	
}

void
sse_log_core(sse_uint_t level, sse_log_t *log, sse_bool_t errno_flag, int error, const char *fmt, va_list args)
{
	char *level_str;
	size_t size, offset;
	time_t t;
	struct tm *tmp;

	switch (level) {
		case SSE_LOG_FATAL:
			level_str = "[FATAL]\t";
			break;
		case SSE_LOG_ERROR:
			level_str = "[ERROR]\t";
			break;
		case SSE_LOG_WARN:
			level_str = "[WARN]\t";
			break;
		case SSE_LOG_INFO:
			level_str = "[INFO]\t";
			break;
		case SSE_LOG_DEBUG:
			level_str = "[DEBUG]\t";
			break;
		default:
			level_str = "[]\t";
			break;
	}

	/* append level */
	size = sse_strlen(level_str);
	sse_strncpy(log_buf, level_str, size);
	offset = size;

	/* append time */
	time(&t);
	tmp = localtime(&t);
	offset += strftime(log_buf + offset, SSE_LOG_BUF_DEFAULT_SIZE - offset - 1, "[%F %T]\t", tmp);

	if (errno_flag == SSE_BOOL_TRUE) {
		offset += snprintf(log_buf + offset,
					SSE_LOG_BUF_DEFAULT_SIZE - offset - 1,
					"[errno: %s]\t",
					strerror(error));
	}

	vsnprintf(log_buf + offset, SSE_LOG_BUF_DEFAULT_SIZE - offset - 1, fmt, args);

	strcat(log_buf, "\n");
	fputs(log_buf, log->log_file);
	fflush(log->log_file);
}
