/*
 * transfer2.c
 *
 * Copyright (C) Lingfei Hu
 */

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define false   0
#define true    1

#define CBUF_CAPACITY   32
#define ZERO_BYTE       '\0'
#define BUF_SIZE        128
#define QUIT            "QUIT"
/*
 *  try to copy in zero-terminated string
 *  return amount that was copied in
 */
#define COPY_BYTE_IN(x)        \
  {                \
    cbuf_buffer[to] = (x);     \
    if (to == CBUF_CAPACITY-1) {        \
      to = 0;                   \
    } else {                    \
      to++;                 \
    }                       \
  }
typedef unsigned char bool;

static void cbuf_init();
static void cbuf_terminate();
int cbuf_data_is_available();
int cbuf_space_available();
static int cbuf_copy_in(char *);
static int cbuf_copy_out(char *);

static void usage(void);
static void err_exit(const char *, ...);
void *fill_func(void *);
void *drain_func(void *);

char *cbuf_buffer;             /* the buffer */
int cbuf_amount;               /* amount of data in the buffer */
int cbuf_first_data;           /* index of first byte that is data */
int cbuf_first_free;           /* index of first byte that is free */


FILE *input_file;
FILE *output_file;
unsigned int fill_usec;
unsigned int drain_usec;

sem_t lock;
sem_t spaces;
sem_t items;

int
main(int argc, char **argv)
{
    char        *input_file_path, *output_file_path;
    int         fill_sleep_time, drain_sleep_time;
    pthread_t   fill_thread, drain_thread;

    if (argc != 5) {
        usage();
        exit(EXIT_FAILURE);
    }

    fill_sleep_time = atoi(argv[3]);
    drain_sleep_time = atoi(argv[4]);

    if (fill_sleep_time < 0 || fill_sleep_time > 1000000) {
        usage();
        exit(EXIT_FAILURE);
    }
    if (drain_sleep_time < 0 || drain_sleep_time > 1000000) {
        usage();
        exit(EXIT_FAILURE);
    }
    fill_usec = (unsigned int)fill_sleep_time;
    drain_usec = (unsigned int)drain_sleep_time;

    input_file_path = argv[1];
    output_file_path = argv[2];

    /* open files */
    input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        err_exit("fopen %s error: %s\n", input_file_path, strerror(errno));
    }

    output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        err_exit("fopen %s error: %s\n", output_file_path, strerror(errno));
    }

    /* init buffer */
    cbuf_init();
    printf("buffer size = %d\n", cbuf_space_available());

    /* init lock */
    if (sem_init(&lock, 0, 1) < 0) {
        err_exit("sem_init error: %s\n", strerror(errno));
    }
    if (sem_init(&spaces, 0, 1) < 0) {
        err_exit("sem_init error: %s\n", strerror(errno));
    }
    if (sem_init(&items, 0, 0) < 0) {
        err_exit("sem_init error: %s\n", strerror(errno));
    }

    /* create threads */
    if (pthread_create(&fill_thread, NULL, fill_func, NULL) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }
    if (pthread_create(&drain_thread, NULL, drain_func, NULL) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }

    /* wait for termination of threads */
    if (pthread_join(fill_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }
    if (pthread_join(drain_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }

    /* clean up */
    if (sem_destroy(&lock) < 0) {
        err_exit("sem_destory error: %s\n", strerror(errno));
    }
    if (sem_destroy(&spaces) < 0) {
        err_exit("sem_destory error: %s\n", strerror(errno));
    }
    if (sem_destroy(&items) < 0) {
        err_exit("sem_destory error: %s\n", strerror(errno));
    }
    cbuf_terminate();
    fclose(input_file);
    fclose(output_file);

    exit(EXIT_SUCCESS);

}

void *
fill_func(void *args)
{
    ssize_t     nread;
    char        *buf;
    size_t      buf_size;
    bool        is_eof, is_written, is_tried;
    int         len;

    buf_size = BUF_SIZE;
    buf = (char *)malloc(sizeof(char) * buf_size);
    if (buf == NULL) {
        err_exit("malloc error\n");
    }

    is_eof = false;
    clearerr(input_file);
    while (1) {
        nread = getline(&buf, &buf_size, input_file);
        
        /* read error */
        if (nread == -1 && ferror(input_file) != 0) {
            err_exit("getline error: %s\n", strerror(errno));
        } 

        /* end of file */
        if (nread == -1) {
            strcpy(buf, QUIT);
            nread = strlen(QUIT);
            is_eof = true;
        }

        if (nread + 1 > CBUF_CAPACITY) {
            err_exit("the length of line is greater than the buffer size %d\n", CBUF_CAPACITY);
        }
        
        is_written = false;
        is_tried = false;
        while (1) {
            /* P(spaces) */
            if (is_tried) {
                sem_wait(&spaces);
            }

            /* get lock */
            if (sem_wait(&lock) < 0) {
                err_exit("sem_wait error in fill thread: %s\n", strerror(errno));
            }

            /* no sufficient spaces to insert */
            len = cbuf_copy_in(buf);
            if (len == 0) {
                printf("fill thread: could not write [%s] -- not enough space (%d)\n", buf, CBUF_CAPACITY - cbuf_amount);
                is_tried = true;
            }
            /* insert successfully */
            else {
                is_written = true;
                printf("fill thread: wrote [%s] into buffer (nwritten=%d)\n", buf, len);
                /* V(items) */
                sem_post(&items);
            }
        
            /* drop lock */
            if (sem_post(&lock) < 0) {
                err_exit("sem_post error in fill thread: %s\n", strerror(errno));
            }

            if (is_written) {
                break;
            } 
            
            usleep(fill_usec);
        }

        if (is_eof) {
            break;
        }
    }

    free(buf);

    return NULL;
}

void *
drain_func(void *args)
{
    char        *buf;
    size_t      buf_size, len;
    int         nread;
    bool        is_eof;
    
    buf_size = BUF_SIZE;
    buf = (char *)malloc(sizeof(char) * buf_size);
    if (buf == NULL) {
        err_exit("malloc error\n");
    }

    is_eof = false;
    clearerr(output_file);
    while (1) {
        /* P(items) */
        sem_wait(&items);

        /* get lock */
        if (sem_wait(&lock) < 0) {
            err_exit("sem_wait error in fill thread: %s\n", strerror(errno));
        }

        /* read line from buffer */
        nread = cbuf_copy_out(buf);
        printf("drain thread: read [%s] from buffer (nread=%d)\n", buf, nread);
        
        /* V(spaces) */
        sem_post(&spaces);
        if (strcmp(buf, QUIT) == 0) {
            is_eof = true;
        }
        len = nread - 1;
        if (!is_eof) {
            if (fwrite(buf, 1, len, output_file) != len) {
                err_exit("fwrite error\n");
            }
        }
        
        /* drop lock */
        if (sem_post(&lock) < 0) {
            err_exit("sem_post error in fill thread: %s\n", strerror(errno));
        }
        
        if (is_eof) {
            break;
        }
        
        usleep(drain_usec); 
    }

    free(buf);

    return NULL;
}


void 
cbuf_init() 
{
  cbuf_buffer = calloc(CBUF_CAPACITY, 1);
  cbuf_amount = 0;
  cbuf_first_data = 0;
  cbuf_first_free = 0;
}


void 
cbuf_terminate() 
{
  free(cbuf_buffer);
}

int 
cbuf_data_is_available() 
{
  return (cbuf_amount > 0);
}


int 
cbuf_space_available() 
{
  return (CBUF_CAPACITY - cbuf_amount);
}

int 
cbuf_copy_in(char *string) 
{
  int to, from;

  if (strlen(string)+1 > (CBUF_CAPACITY - cbuf_amount))
    return 0;
  to = cbuf_first_free;
  for (from=0; from<strlen(string); from++)
    COPY_BYTE_IN(string[from]);
  COPY_BYTE_IN(ZERO_BYTE);
  cbuf_first_free = to;
  cbuf_amount = cbuf_amount + strlen(string) + 1;
  return strlen(string) + 1;
}


/*
 *  copy out next zero-terminated string
 *  return amount that was copied out
 */
int 
cbuf_copy_out(char *data) 
{
  int from, amount;
  char ch;

  if (cbuf_amount == 0)
    return 0;
  from = cbuf_first_data;
  amount = 0;
  do {
    ch = cbuf_buffer[from];
    data[amount++] = ch;
    if (from == CBUF_CAPACITY-1) {
      from = 0;
    } else {
      from++;
    }
  } while (ch != ZERO_BYTE);
  cbuf_amount = cbuf_amount - amount;
  cbuf_first_data = from;
  return amount;
}

static void
usage(void)
{
    printf("usage: ./rw [input file] [output file] [sleep time of fill] [sleep time of drain]]\n");
    printf("The range of sleep time of fill and drain must be [0, 0,1000000]\n");
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

