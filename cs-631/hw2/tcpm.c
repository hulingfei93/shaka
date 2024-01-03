/*
 * tcpm 1.0 - trivially copy a file via mmap/memcpy
 *
 * Copyright (C) Lingfei Hu
 */
#include <sys/stat.h>
#include <sys/mman.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void usage(void);
static void err_exit(const char *, ...);
static char *file_path_combine(const char *, const char *);
int main(int, char **);

#define DEFAULT_PAGE_SIZE		4096

/*
 * The tcpm utility copies the contents of the source to target.
 *
 * Examples:
 * tcpm file1 file2
 * tcpm file1 dir
 * tcpm file1 dir/file2
 * tcpm file1 dir/subdir/subsubdir/file2
 */

int
main(int argc, char **argv) 
{
	if (argc != 3) {
		usage();
		exit(EXIT_FAILURE);
	}

	char			*source, *target, *dest;
	struct stat		stat_buf, stat_buf1;

	source = argv[1];
	target = argv[2];
	
	/* 
	 * validation for source argument 
	 */
	if (stat(source, &stat_buf) < 0) {
		err_exit("tcpm: cannot stat '%s': %s\n", 
			source, strerror(errno));
	}

	if (!S_ISREG(stat_buf.st_mode)) {
		err_exit("tcpm: '%s' doesn't point to a regular file'\n", 
			source);
	}

	if (access(source, R_OK) < 0) {
		err_exit("tcpm: '%s': no read permission\n", source);
	}

	/* 
	 * validation for target argument 
	 */
	/* target exists*/
	if (access(target, F_OK) == 0) {
		if (stat(target, &stat_buf1) < 0) {
			err_exit("tcpm: cannot stat '%s': %s\n", 
				target, strerror(errno));
		}
		/* target is a regular file */
		if (S_ISREG(stat_buf1.st_mode)) {
			dest = target;
		} else if (S_ISDIR(stat_buf1.st_mode)) {  /* target is a directory */
			dest = file_path_combine(target, basename(source));		
		} else {
			err_exit("tcpm: '%s' isn't a regular file or a directory'\n", 
				target);
		}
	} else {  /* target doesn't exist */
		dest = target;
	}
	/* if source and destination point to the same file, just exit with 0 */
	if (access(dest, F_OK) == 0) {
		if (stat(dest, &stat_buf1) < 0) {
			err_exit("tcpm: cannot stat '%s': %s\n", 
				dest, strerror(errno));
		}
		if (stat_buf.st_dev == stat_buf1.st_dev 
				&& stat_buf.st_ino == stat_buf1.st_ino) {
			exit(EXIT_SUCCESS);				
		}
	}

	/* 
	 * do copy 
	 */
	int				src_fd, dest_fd;
	struct stat		src_stat;
	off_t			pos;
	size_t			map_len;
	void			*src_addr, *dest_addr;
	long			page_size;

	pos = 0;

	/* get system memory page size */
	page_size =	sysconf(_SC_PAGE_SIZE);
	if (page_size < 0) {
		page_size = DEFAULT_PAGE_SIZE;
	}

	if ((src_fd = open(source, O_RDONLY)) == -1) {	
		err_exit("tcpm: cannot open '%s': %s\n", 
			source, strerror(errno));
	}
	
	if ((dest_fd = open(dest, O_RDWR | O_CREAT | O_TRUNC, 
			stat_buf.st_mode)) == -1) {	
		err_exit("tcpm: cannot open '%s': %s\n", 
			dest, strerror(errno));
	}

	if (fstat(src_fd, &src_stat) < 0) {
		err_exit("tcpm: cannot fstat '%s': %s\n", 
			source, strerror(errno));
	}
	if (ftruncate(dest_fd, src_stat.st_size) < 0) {
		err_exit("tcpm: cannot ftruncate '%s': %s\n", 
			dest, strerror(errno));
	}

	while (pos < src_stat.st_size) {
		if (src_stat.st_size - pos > page_size) {
			map_len = page_size;
		} else {
			map_len = src_stat.st_size - pos;
		}

		if ((src_addr = mmap(0, map_len, PROT_READ, 
				MAP_SHARED, src_fd, pos)) == MAP_FAILED) {	
			err_exit("tcpm: mmap error for '%s': %s\n", 
				source, strerror(errno));
		}
		if ((dest_addr = mmap(0, map_len, PROT_READ | PROT_WRITE, 
				MAP_SHARED, dest_fd, pos)) == MAP_FAILED) {	
			err_exit("tcpm: mmap error for '%s': %s\n", 
				dest, strerror(errno));
		}

		memcpy(dest_addr, src_addr, map_len);
		munmap(src_addr, map_len);
		munmap(dest_addr, map_len);
		
		pos += map_len;
	}


	close(src_fd);
	close(dest_fd);
	
	exit(EXIT_SUCCESS);
}

void
usage(void)
{
	printf("Usage: tcpm source target\n");
	printf("Copy the contents of the source to target\n");
}

void
err_exit(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	exit(EXIT_FAILURE);
}

char *
file_path_combine(const char *dir, const char *name)
{
	size_t		dir_size, name_size, size;
	char		*path;

	dir_size = strlen(dir);
	name_size = strlen(name);
	size = dir_size + name_size;

	if (dir[dir_size - 1] != '/') {
		++size;
	}
	
	path = (char *)malloc(sizeof(char) * (size + 1));
	strcpy(path, dir);
	if (dir[dir_size - 1] != '/') {
		path[dir_size++] = '/';
	}
	strcpy(path + dir_size, name);

	return path;
}
