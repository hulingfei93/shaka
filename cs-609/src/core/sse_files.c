#include <sse_core.h>

void
sse_file_path_fill(char *path, const char *dir, const char *name, const char *ext)
{
	size_t cur_size;

	sse_strcpy(path, dir);
	cur_size = sse_strlen(dir);
	if (path[cur_size - 1] != '/') {
		path[cur_size++] = '/';
	}

	sse_strcpy(path + cur_size, name);
	cur_size += sse_strlen(name);

	sse_strcpy(path + cur_size, ext);
}

sse_fd_t
sse_open_file(const char *path, sse_int_t flag, sse_file_mode_t mode)
{
	umask(0);
	return open(path, flag, mode);
}

sse_bool_t
sse_dir_create(const char *path)
{
	if (sse_dir_is_exist(path))
	  return SSE_BOOL_TRUE;

	char dir_path[SSE_PATH_MAX_SIZE];
	size_t len, i;

	sse_strcpy(dir_path, path);
	len = sse_strlen(dir_path);
	if (dir_path[len - 1] != '/') {
		dir_path[len++] = '/';
		dir_path[len] = '\0';
	}

	for (i = 1; i < len; ++i) {
		if (dir_path[i] == '/') {
			dir_path[i] = '\0';
			if (!sse_dir_is_exist(dir_path)) {
				if (mkdir(dir_path, SSE_DIR_DEFAULT_MODE) != 0) {
					return SSE_BOOL_FALSE;
				}
			}
			dir_path[i] = '/';
		}
	}
	
	return SSE_BOOL_TRUE;

}

void
sse_dir_remove(const char *path)
{
	sse_dir_cleanup(path);

	if (sse_dir_is_exist(path)) {
		rmdir(path);
	}
}

void
sse_dir_cleanup(const char *path)
{
	if (!sse_dir_is_exist(path)) {
		return;
	}
	
	DIR					*dir;
	struct dirent		*dirp;
	char				path_buf[SSE_PATH_MAX_SIZE];
	size_t				dir_len;
	struct stat			statbuf;

	dir_len = sse_strlen(path);
	sse_strcpy(path_buf, path);
	if (path_buf[dir_len - 1] != '/') {
		path_buf[dir_len++] = '/';
		path_buf[dir_len] = '\0';
	}

	dir = opendir(path);
	while ((dirp = readdir(dir)) != SSE_NULL) {
		sse_strcpy(path_buf + dir_len, dirp->d_name);

		if (stat(path_buf, &statbuf) < 0) {
			continue;
		}

		if (!S_ISREG(statbuf.st_mode)) {
			continue;
		}
		unlink(path_buf);
	}
	closedir(dir);
}

sse_bool_t
sse_dir_is_exist(const char *path)
{
	if (access(path, F_OK) == 0) {
		return SSE_BOOL_TRUE;
	}
	return SSE_BOOL_FALSE;
}

sse_bool_t
sse_file_is_exist(const char *path)
{
	if (access(path, F_OK) == 0) {
		return SSE_BOOL_TRUE;
	}
	return SSE_BOOL_FALSE;
}

sse_bool_t
sse_file_rename(const char *old_name, const char *new_name)
{
	if (sse_file_is_exist(new_name) || !sse_file_is_exist(old_name)) {
		return SSE_BOOL_FALSE;
	}
	if (rename(old_name, new_name) != 0) {
		return SSE_BOOL_FALSE;
	}
	return SSE_BOOL_TRUE;
}

sse_bool_t
sse_file_remove(const char *path)
{
	if (!sse_file_is_exist(path)) {
		return SSE_BOOL_TRUE;
	}
	if (unlink(path) == 0) {
		return SSE_BOOL_TRUE;
	}
	return SSE_BOOL_FALSE;
}

sse_bool_t
sse_file_copy(const char *dest_path, const char *src_path)
{
	char			buf[SSE_FILE_COPY_BUF_SIZE];
	sse_ssize_t		n;
	sse_fd_t		from, to;

	from = open(src_path, SSE_FILE_RDONLY);
	if (from == -1) {
		return SSE_BOOL_FALSE;
	}
	to = open(dest_path, SSE_FILE_WRONLY | SSE_FILE_TRUNCATE, SSE_FILE_DEFAULT_MODE);
	if (to == -1) {
		close(from);
		return SSE_BOOL_FALSE;
	}

	while ((n = read(from, buf, SSE_FILE_COPY_BUF_SIZE)) > 0) {
		if (write(to, buf, n) != n) {
			close(from);
			close(to);
			return SSE_BOOL_FALSE;
		}
	}

	if (n < 0) {
		close(from);
		close(to);
		return SSE_BOOL_FALSE;
	}

	close(from);
	close(to);
	return SSE_BOOL_TRUE;
}

/******************stdio interface*******************/
void
sse_stdio_write_vuint(sse_uint_t i, FILE *file)
{
	unsigned char c;

	while (i & ~0x7fUL) {
		c = (i & 0x7fUL) | 0x80UL;
		putc(c, file);
		i >>= 7;
	}
	c = i;
	putc(c, file);
}

void 
sse_stdio_write_uint(sse_uint_t i, FILE *file)
{
	fwrite(&i, sizeof(sse_uint_t), 1, file);
}

void
sse_stdio_write_float(float f, FILE *file)
{
	fwrite(&f, sizeof(float), 1, file);
}

sse_uint_t
sse_stdio_read_vuint(FILE* file)
{
	sse_uint_t i;
	int c, shift;

	c = getc(file);
	i = c & 0x7f;

	for (shift = 7; c & 0x80; shift += 7) {
		c = getc(file);
		i |= (c & 0x7f) << shift;
	}
	return i;
}

sse_uint_t
sse_stdio_read_uint(FILE *file)
{
	sse_uint_t i;
	fread(&i, sizeof(sse_uint_t), 1, file);
	return i;
}

float
sse_stdio_read_float(FILE *file)
{
	float f;
	fread(&f, sizeof(float), 1, file);
	return f;
}

size_t
sse_stdio_write(void *buf, size_t size, FILE *file)
{
	return fwrite(buf, 1, size, file);
}

size_t
sse_stdio_read(void *buf, size_t size, FILE *file)
{
	return fread(buf, 1, size, file);
}
