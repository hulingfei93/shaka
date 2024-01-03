/*
 * sish_core.h
 */

#ifndef _SISH_CORE_H_INCLUDED_
#define _SISH_CORE_H_INCLUDED_

#include <sys/stat.h>
#include <sys/wait.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SISH_NULL	NULL

#define SISH_FD_STDIN	STDIN_FILENO
#define SISH_FD_STDOUT	STDOUT_FILENO
#define SISH_FD_STDERR	STDERR_FILENO
#define SISH_FILE_MODE	0644

#define SISH_EXIT_SUCCESS	0
#define SISH_EXIT_FAILURE	127

#define SISH_LINE_BUF_SIZE			1024
#define SISH_POOL_NORMAL_SIZE		1024
#define SISH_VECTOR_SMALL_SIZE		5
#define SISH_VECTOR_NORMAL_SIZE		10
#define SISH_VECTOR_LARGE_SIZE		20


#define false	0
#define true	1

#define sish_abs(value)			(((value) >= 0) ? (value) : -(value))	
#define sish_max(val1, val2)		(((val1) < (val2)) ? (val2) : (val1))	
#define sish_min(val1, val2)		(((val1) > (val2)) ? (val2) : (val1))

#define sish_tolower(c)      ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define sish_toupper(c)      ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

/* boolean */
typedef unsigned char sish_bool_t;

/* integer */
typedef short sish_short_t;					/* at least 16 bits [-32767, +32767] */
typedef unsigned short sish_ushort_t;		
typedef int sish_int_t;						/* at least 16 bits [-32767, +32767] */
typedef unsigned int sish_uint_t;
typedef long sish_int32_t;					/* at least 32 bits [-2147483647, +2147483647] */
typedef unsigned long sish_uint32_t;
typedef long long sish_int64_t;				/* at least 64 bits [-9223372036854775807, +9223372036854775807] */
typedef unsigned long long sish_uint64_t;

typedef int			sish_fd_t;
typedef size_t		sish_size_t;
typedef ssize_t		sish_ssize_t;
typedef mode_t		sish_mode_t;
typedef pid_t		sish_pid_t;

#include <sish_alloc.h>
#include <sish_log.h>
#include <sish_pool.h>
#include <sish_vector.h>

#include <sish_parser.h>
#include <sish_process.h>

#endif /* _SISH_CORE_H_INCLUDED_ */

