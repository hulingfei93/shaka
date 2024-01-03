#ifndef _SSE_CORE_H_INCLUDED_
#define _SSE_CORE_H_INCLUDED_

#define SSE_OK           0
#define SSE_ERROR       -1
#define SSE_ABORT       -2

#define SSE_NULL        NULL

#define sse_abs(value)          (((value) >= 0) ? (value) : -(value))   
#define sse_max(val1, val2)     (((val1) < (val2)) ? (val2) : (val1))   
#define sse_min(val1, val2)     (((val1) > (val2)) ? (val2) : (val1))   

#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <malloc.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <math.h>
#include <lz4.h>

/* integer */
typedef short sse_short_t;                  /* at least 16 bits [-32767, +32767] */
typedef unsigned short sse_ushort_t;        
typedef int sse_int_t;                      /* at least 16 bits [-32767, +32767] */
typedef unsigned int sse_uint_t;
typedef long sse_int32_t;                   /* at least 32 bits [-2147483647, +2147483647] */
typedef unsigned long sse_uint32_t;
typedef long long sse_int64_t;              /* at least 64 bits [-9223372036854775807, +9223372036854775807] */
typedef unsigned long long sse_uint64_t;

/* boolean */
#define SSE_BOOL_FALSE  0
#define SSE_BOOL_TRUE   1

typedef unsigned char sse_bool_t;
typedef size_t sse_size_t;
typedef ssize_t sse_ssize_t;
typedef mode_t sse_file_mode_t;

#define SSE_MODE_NORMAL         0
#define SSE_MODE_DEBUG          1

/* log */
#define SSE_LOG_OFF             0
#define SSE_LOG_FATAL           1
#define SSE_LOG_ERROR           2
#define SSE_LOG_WARN            3
#define SSE_LOG_INFO            4
#define SSE_LOG_DEBUG           5
#define SSE_LOG_ALL             256

#define SSE_LOG_BUF_DEFAULT_SIZE    4096
#define SSE_POOL_DEFAULT_SIZE       4096
#define SSE_POOL_LARGE_SIZE         1024*1024*10

#include <sse_string.h>
#include <sse_files.h>
#include <sse_log.h>
#include <sse_alloc.h>
#include <sse_pool.h>
#include <sse_hash.h>
#include <sse_rbtree.h>
#include <sse_vector.h>
#include <sse_tstree.h>
#include <sse_heap.h>

#endif /* _SSE_CORE_H_INCLUDED_ */
