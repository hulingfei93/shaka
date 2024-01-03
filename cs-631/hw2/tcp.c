/*
 * tcp 1.0 - trivially copy a file
 *
 * Copyright (C) Lingfei Hu
 */

#include <sys/stat.h>

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

#define BUF_SIZE        4096

/*
 * The tcp utility copies the contents of the source to target
 * 
 * If target is a directory, tcp will copy source into this directory.
 *
 * Examples:
 * tcp file1 file2
 * tcp file1 dir
 * tcp file1 dir/file2
 * tcp file1 dir/subdir/subsubdir/file2
 */

int
main(int argc, char **argv) 
{
    if (argc != 3) {
        usage();
        exit(EXIT_FAILURE);
    }

    char            *source, *target, *dest;
    struct stat     stat_buf, stat_buf1;

    source = argv[1];
    target = argv[2];
    
    /* 
     * validation for source argument 
     */
    if (stat(source, &stat_buf) < 0) {
        err_exit("tcp: cannot stat '%s': %s\n", 
                    source, strerror(errno));
    }

    if (!S_ISREG(stat_buf.st_mode)) {
        err_exit("tcp: '%s' doesn't point to a regular file'\n", 
            source);
    }

    if (access(source, R_OK) < 0) {
        err_exit("tcp: '%s': no read permission\n", source);
    }

    /* 
     * validation for target argument 
     */
    /* target exists*/
    if (access(target, F_OK) == 0) {
        if (stat(target, &stat_buf1) < 0) {
            err_exit("tcp: cannot stat '%s': %s\n", 
                target, strerror(errno));
        }
        /* target is a regular file */
        if (S_ISREG(stat_buf1.st_mode)) {
            dest = target;
        } else if (S_ISDIR(stat_buf1.st_mode)) { /* target is a directory*/
            dest = file_path_combine(target, basename(source));     
        } else {
            err_exit("tcp: '%s' isn't a regular file or a directory'\n", 
                target);
        }
    } else {  /* target doen't exist */
        dest = target;
    }
    /* if source and destination point to the same file, just exit with 0 */
    if (access(dest, F_OK) == 0) {
        if (stat(dest, &stat_buf1) < 0) {
            err_exit("tcp: cannot stat '%s': %s\n", 
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
    int     src_fd, dest_fd, n;
    char    buf[BUF_SIZE];

    if ((src_fd = open(source, O_RDONLY)) == -1) {  
        err_exit("tcp: cannot open '%s': %s\n", 
            source, strerror(errno));
    }
    
    if ((dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 
            stat_buf.st_mode)) == -1) { 
        err_exit("tcp: cannot open '%s': %s\n", 
            dest, strerror(errno));
    }

    while ((n = read(src_fd, buf, BUF_SIZE)) > 0) {
        if (write(dest_fd, buf, n) != n) {
            err_exit("tcp: write error at '%s': %s\n", 
                dest, strerror(errno));
        }
    }

    close(src_fd);
    close(dest_fd);

    if (n < 0) {
        err_exit("tcp: read error at '%s': %s\n", 
            source, strerror(errno));
    }
    
    exit(EXIT_SUCCESS);
}

void
usage(void)
{
    printf("Usage: tcp source target\n");
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
    size_t      dir_size, name_size, size;
    char        *path;

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

