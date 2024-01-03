/*
 * primes.c
 *
 * Copyright (C) Lingfei Hu
 */

#include <sys/select.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define false   0
#define true    1

#define WRITE_BUF_SIZE      32
#define READ_BUF_SIZE       128
#define SMALL_BUF_SIZE      32

#define FIFO_PATH_PREFIX    "/tmp/.algdfyufb_"
#define FIFO_PATH_MAX_SIZE  128

typedef unsigned char bool;

/*
 * buf has been used to save a tail segment of read buffer, 
 * if the tail of read buffer cannot contain the a whole prime
 *
 * For example, for read buffer with "61\067\07", the next prime
 * number 71 is only read partially.
 */
typedef struct {
    int             id;
    int             fd[2];
    int             bottom;
    int             top;
    int             prime_count;
    size_t          buf_size;
    int             buf_pos;
    pid_t           pid;
    char            *fifo_path;
    char            *buf;
} process_t;

static void usage(void);
static void err_exit(const char *, ...);
static void err_log(const char *, ...);
static bool is_prime(int);
static void prime_process(process_t *, int);
static void print_results(char *, size_t, process_t *);
int main(int, char **);

/*
 * A C program named "primes"" that accepts 
 * a sequence of increasing positive integers 
 * on its command line. The integers define ranges, 
 * and a child process must be created to search 
 * for primes in each range.
 *
 * For example:
 * ./primes 44
 * ./primes 44 100
 */
int
main(int argc, char **argv)
{
    int             i, exit_status, exit_code;
    int             child_process_count;
    process_t       *processes;
    process_t       *cur_proc;
    int             max_fd, read_fd_ready_count, read_fd_count;
    fd_set          read_fds, read_fds_save;
    char            read_buf[READ_BUF_SIZE];
    ssize_t         read_size;
    size_t          fifo_path_size;
    
    if (argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }

    child_process_count = argc - 1;
    processes = (process_t *)malloc(sizeof(process_t) * child_process_count);

    /* 
     * init process info 
     */
    for (i = 0; i < child_process_count; ++i) {
        processes[i].id = i + 1;
        if (i == 0) {
            processes[i].bottom = 2;
        } 
        else {
            processes[i].bottom = atoi(argv[i]) + 1;
        }
        processes[i].top = atoi(argv[i + 1]);
        processes[i].prime_count = 0;
        processes[i].buf_size = SMALL_BUF_SIZE;
        processes[i].buf = (char *)malloc(sizeof(char) * processes[i].buf_size);
        processes[i].buf_pos = 0;
    }

    /* 
     * create child processes 
     */
    for (i = 0; i < child_process_count; ++i) {
        /* 
         * Odd-numbered id child processes should use a pipe to communicate with the parent; 
         * even-numbered id child processes should use a FIFO to communicate
         * with the parent.
         */
        /* pipe */
        cur_proc = processes + i;
        if (cur_proc->id % 2 == 1) {
            if (pipe(cur_proc->fd) < 0) {
              err_exit("pipe error: %s\n", strerror(errno));
            }
            cur_proc->pid = fork();
            if (cur_proc->pid < 0) {
              err_exit("fork error: %s\n", strerror(errno));
            } 
            else if (cur_proc->pid == 0) { /* child process*/
                /* close read fd */
                close(cur_proc->fd[0]);
                prime_process(cur_proc, cur_proc->fd[1]);

            } 
            else { /* parent process */
                /* close write fd */
                close(cur_proc->fd[1]);
                printf("child %d: bottom=%d, top=%d\n",
                            cur_proc->pid,
                            cur_proc->bottom,
                            cur_proc->top);
            }
        }
        /* fifo */
        else {
            /* create fifo */
            cur_proc->fifo_path = (char *)malloc(sizeof(char) * FIFO_PATH_MAX_SIZE);
            fifo_path_size = strlen(FIFO_PATH_PREFIX);
            strcpy(cur_proc->fifo_path, FIFO_PATH_PREFIX);
            sprintf(cur_proc->fifo_path + fifo_path_size, "%d", cur_proc->id);
            if (access(cur_proc->fifo_path, F_OK) == 0) {
                unlink(cur_proc->fifo_path);
            } 

            if (mkfifo(cur_proc->fifo_path, S_IRUSR|S_IWUSR) < 0) {
                err_exit("mkfifo error at %s: %s\n", cur_proc->fifo_path, strerror(errno));
            }
            /* create child process */
            cur_proc->pid = fork();
            if (cur_proc->pid < 0) {
              err_exit("fork error: %s\n", strerror(errno));
            } 
            else if (cur_proc->pid == 0) { /* child process*/
                cur_proc->fd[1] = open(cur_proc->fifo_path, O_WRONLY);
                if (cur_proc->fd[1] < 0) {
                    err_exit("open error at %s: %s\n", cur_proc->fifo_path, strerror(errno));
                }
                prime_process(cur_proc, cur_proc->fd[1]);

            } 
            else { /* parent process */
                /* open fifo */
                cur_proc->fd[0] = open(cur_proc->fifo_path, O_RDONLY);
                if (cur_proc->fd[0] < 0) {
                    err_exit("open error at %s: %s\n", cur_proc->fifo_path, strerror(errno));
                }
                /* remove fifo automatically after process exits */
                
                if (unlink(cur_proc->fifo_path) < 0) {
                    err_exit("unlink error at %s: %s\n", cur_proc->fifo_path, strerror(errno));
                }
                printf("child %d: bottom=%d, top=%d\n",
                            cur_proc->pid,
                            cur_proc->bottom,
                            cur_proc->top);
            }

        } 
    }

    /* 
     * the parent uses select to wait for results from all children 
     */
    /* calculate max fd */
    max_fd = 0;
    read_fd_count = 0;
    FD_ZERO(&read_fds_save);
    for (i = 0; i < child_process_count; ++i) {
        cur_proc = processes + i;
        if (cur_proc->fd[0] > max_fd) {
            max_fd = cur_proc->fd[0];
        }
        FD_SET(cur_proc->fd[0], &read_fds_save);
        ++read_fd_count;
    }

    while (1) {
        if (read_fd_count == 0) {
            break;
        }
        read_fds = read_fds_save;
        read_fd_ready_count = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (read_fd_ready_count < 0) {
              err_exit("select error: %s\n", strerror(errno));
        }

        for (i = 0; i < child_process_count && read_fd_ready_count > 0; ++i) {
            cur_proc = processes + i;
            if (FD_ISSET(cur_proc->fd[0], &read_fds)) {
                /* read data */
                read_size = read(cur_proc->fd[0], read_buf, READ_BUF_SIZE);
                if (read_size < 0) {
                    err_log("read error: %s\n", strerror(errno));
                } 
                /* child process existed*/
                else if (read_size == 0) {
                    /* get the exit code */
                    if (waitpid(cur_proc->pid, &exit_status, 0) != cur_proc->pid) {
                        err_log("waitpid: %s\n", strerror(errno));
                    } 
                    else {
                        if (WIFEXITED(exit_status)) {
                            exit_code = WEXITSTATUS(exit_status);
                            if (exit_code == cur_proc->prime_count) {
                                printf("child %d exited correctly\n", cur_proc->pid);
                            }
                            else {
                                printf("the child %d exit code %d doesn't match the actual produced prime number %d\n",
                                            cur_proc->pid,
                                            exit_code,
                                            cur_proc->prime_count);
                            }
                        } 
                        else {
                            printf("child %d exited abnormally\n",
                                cur_proc->pid);
                        }
                    }
                    
                    /* clean up fd from fd set */
                    FD_CLR(cur_proc->fd[0], &read_fds_save);
                    --read_fd_count;
                }
                /* calculate primes and write the results */
                else {
                    print_results(read_buf, read_size, cur_proc);
                }
                --read_fd_ready_count;
                
            } /* end if */
        } /* end for */
    } /* end while */


    exit(EXIT_SUCCESS);
}

/*
 * print the results from buffer
 */
static void
print_results(char *buf, size_t size, process_t *proc) 
{   
    char    *begin, *end;
    int     pos;

    begin = end = buf;
    pos = 0;

    if (proc->buf_pos > 0) {
        while (pos < size && *end != '\0') {
            proc->buf[proc->buf_pos++] = *end++;
            ++pos;
        }
        if (pos < size) {
            proc->buf[proc->buf_pos++] = *end++;
            ++pos;
        }
        /* print prime saved in the process buffer */
        printf("%s is prime\n", proc->buf);
        ++proc->prime_count;
        proc->buf_pos = 0;

        begin = end;
    }

    while (pos < size) {
        if (*end == '\0') {
            printf("%s is prime\n", begin);
            ++proc->prime_count;
            begin = end + 1;
        }
        ++end;
        ++pos;
    }
    /* save the incompleted data to process buffer */
    if (begin != end) {
        memcpy(proc->buf, begin, end - begin);
        proc->buf_pos = end - begin;
    }
}

/*
 * calculate primes and write to buffer
 */
static void
prime_process(process_t *proc, int write_fd)
{
    int     i, prime_count, size;
    char    write_buf[WRITE_BUF_SIZE];

    
    prime_count = 0;
    for (i = proc->bottom; i <= proc->top; ++i) {
        if (is_prime(i)) {
            size = snprintf(write_buf, WRITE_BUF_SIZE, "%d", i);
            if (size < 0) {
              err_exit("snprintf error\n");
            }
            /* write '\0' to the buffer */
            ++size;
            if (write(write_fd, write_buf, (size_t)size) != size) {
              err_exit("write error: %s\n", strerror(errno));
            }
            ++prime_count;
        }
    }
    /* close write fd */
    close(write_fd);

    exit(prime_count);
}

static void
usage(void)
{
    printf("usage: ./primes <increasing positive integers>\n");
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

static bool
is_prime(int num)
{
    int i;
    
    if (num <= 1) {
        return false;
    }

    for (i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

