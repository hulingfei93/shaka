#ifndef _SSE_FILES_H_INCLUDED_
#define _SSE_FILES_H_INCLUDED_

#include <sse_core.h>

typedef int		sse_fd_t;

#define SSE_FD_STDIN		STDIN_FILENO	
#define SSE_FD_STDOUT		STDOUT_FILENO
#define SSE_FD_STDERR		STDERR_FILENO
#define SSE_FD_INVALID		-1

#define SSE_STDIO_STDIN		stdin
#define SSE_STDIO_STDOUT	stdout
#define SSE_STDIO_STDERR	stderr

#define SSE_FILE_RDONLY				O_RDONLY
#define SSE_FILE_WRONLY				O_WRONLY
#define SSE_FILE_RDWR				O_RDWR
#define SSE_FILE_CREATE_OR_OPEN		O_CREATE
#define SSE_FILE_OPEN				0
#define SSE_FILE_TRUNCATE			(O_CREAT|O_TRUNC)
#define SSE_FILE_APPEND				(O_WRONLY|O_APPEND)

#define SSE_FILE_DEFAULT_MODE		0644
#define SSE_DIR_DEFAULT_MODE		0755
#define SSE_PATH_MAX_SIZE			1024
#define SSE_INT_MAX_SIZE			32

#define SSE_FILE_COPY_BUF_SIZE		8192

#define sse_file_write(fd, buf, size)	write(fd, buf, size)

void sse_file_path_fill(char *path, const char *dir, const char *name, const char *ext);
sse_fd_t sse_open_file(const char *path, sse_int_t flag, sse_file_mode_t mode);

sse_bool_t sse_dir_create(const char *path);
void sse_dir_remove(const char *path);
void sse_dir_cleanup(const char *path);
sse_bool_t sse_dir_is_exist(const char *path);

sse_bool_t sse_file_is_exist(const char *path);
sse_bool_t sse_file_rename(const char *old_name, const char *new_name);
sse_bool_t sse_file_remove(const char *path);
sse_bool_t sse_file_copy(const char *dest_path, const char *src_path);

void sse_stdio_write_vuint(sse_uint_t i, FILE *file);
void sse_stdio_write_uint(sse_uint_t i, FILE *file);
void sse_stdio_write_float(float f, FILE *file);
sse_uint_t sse_stdio_read_vuint(FILE* file);
sse_uint_t sse_stdio_read_uint(FILE *file);
float sse_stdio_read_float(FILE *file);
size_t sse_stdio_write(void *buf, size_t size, FILE *file);
size_t sse_stdio_read(void *buf, size_t size, FILE *file);

#endif /* _SSE_FILES_H_INCLUDED_ */
