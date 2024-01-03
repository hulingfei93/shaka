/*
 * rw.c
 *
 * Copyright (C) Lingfei Hu
 */

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_INIT_SIZE	128

static void usage(void);
static void err_exit(const char *, ...);
static void err_log(const char *, ...);
int main(int, char **);

/*
 * a single-threaded program named "rw" that repeatedly reads 
 * a line of text from an input file, writes the line into a buffer, 
 * then writes the line from the buffer to an output file.
 */
int
main(int argc, char **argv)
{
	char		*input_file_path, *output_file_path;
	FILE		*input_file, *output_file;
	ssize_t		nread;
	char		*buf;
	size_t		buf_size;

	if (argc != 3) {
		usage();
		exit(EXIT_FAILURE);
	}

	input_file_path = argv[1];
	output_file_path = argv[2];
	buf_size = BUF_INIT_SIZE;
	buf = (char *)malloc(sizeof(char) * buf_size);
	if (buf == NULL) {
		err_exit("malloc error\n");
	}

	/* open files */
	input_file = fopen(input_file_path, "r");
	if (input_file == NULL) {
		err_exit("fopen %s error: %s\n", input_file_path, strerror(errno));
	}

	output_file = fopen(output_file_path, "w");
	if (output_file == NULL) {
		err_exit("fopen %s error: %s\n", output_file_path, strerror(errno));
	}

	/* do copy */
	clearerr(input_file);
	while ((nread = getline(&buf, &buf_size, input_file)) != -1) {
		if (fwrite(buf, 1, nread, output_file) != nread) {
			err_exit("fwrite error\n");
		}
	}

	/* read error */
	if (ferror(input_file) != 0) {
		err_log("getline error: %s\n", strerror(errno));
	}

	/* clean up */
	free(buf);
	fclose(input_file);
	fclose(output_file);

	exit(EXIT_SUCCESS);

}

static void
usage(void)
{
	printf("usage: ./rw [input file] [output file]\n");
}

static void
err_exit(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	exit(EXIT_FAILURE);
}

static void
err_log(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}
