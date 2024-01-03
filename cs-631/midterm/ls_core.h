/*
 * ls_core.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_CORE_H_INCLUDED_
#define _LS_CORE_H_INCLUDED_

#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <string.h>
#include <bsd/string.h>
#include <time.h>
#include <unistd.h>

#define LS_NULL			NULL

#define LS_EXIT_SUCCESS		EXIT_SUCCESS
#define LS_EXIT_FAILURE		EXIT_FAILURE

#define LS_POOL_SMALL_SIZE		256
#define LS_POOL_NORMAL_SIZE		4096
#define LS_VECTOR_NORMAL_SIZE	20

#define LS_BOOL_FALSE	0
#define LS_BOOL_TRUE	1

#define LS_CUR_DIR		"."

#define ls_abs(value)			(((value) >= 0) ? (value) : -(value))	
#define ls_max(val1, val2)		(((val1) < (val2)) ? (val2) : (val1))	
#define ls_min(val1, val2)		(((val1) > (val2)) ? (val2) : (val1))

#define ls_tolower(c)      ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ls_toupper(c)      ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

/* string operations */
#define ls_strcpy(d, s)			strcpy((char *) d, (const char *) s)
#define ls_strncpy(d, s, n)		strncpy((char *) d, (const char*) s, (size_t) n)
#define ls_strcat(d, s)			strcat((char *) d, (const char *) s)
#define ls_strncat(d, s, n)		strncat((char *) d, (const char *) s, (size_t) n)
#define ls_strcmp(s1, s2)		strcmp((const char *) s1, (const char *) s2)
#define ls_strncmp(s1, s2, n)	strncmp((const char *) s1, (const char *) s2, (size_t) n)
#define ls_strlen(str)			strlen((const char *) str)

/* memory operations */
#define ls_memcpy(d, s, n)		memcpy(d, s, n)
#define ls_memmove(d, s, n)		memmove(d, s, n)
#define ls_memcmp(s1, s2, n)	memcmp(s1, s2, n)
#define ls_memchr(str, c, n)	memchr(str, c, n)
#define ls_memset(buf, c, n)	memset(buf, c, n)
#define ls_memzero(buf, n)		memset(buf, 0, n)

/* boolean */
typedef unsigned char ls_bool_t;

/* integer */
typedef short ls_short_t;					/* at least 16 bits [-32767, +32767] */
typedef unsigned short ls_ushort_t;		
typedef int ls_int_t;						/* at least 16 bits [-32767, +32767] */
typedef unsigned int ls_uint_t;
typedef long ls_int32_t;					/* at least 32 bits [-2147483647, +2147483647] */
typedef unsigned long ls_uint32_t;
typedef long long ls_int64_t;				/* at least 64 bits [-9223372036854775807, +9223372036854775807] */
typedef unsigned long long ls_uint64_t;

typedef double		ls_double_t;

typedef int			ls_fd_t;
typedef size_t		ls_size_t;
typedef ssize_t		ls_ssize_t;
typedef mode_t		ls_mode_t;
typedef ino_t		ls_ino_t;
typedef nlink_t		ls_nlink_t;
typedef uid_t		ls_uid_t;
typedef gid_t		ls_gid_t;
typedef off_t		ls_off_t;
typedef blkcnt_t	ls_blkcnt_t;
typedef struct timespec		ls_timespec_t;

#include <ls_alloc.h>
#include <ls_log.h>
#include <ls_pool.h>
#include <ls_list.h>
#include <ls_file.h>
#include <ls_vector.h>

#include <ls_process.h>
#include <ls_print.h>

#endif /* _LS_CORE_H_INCLUDED_ */

