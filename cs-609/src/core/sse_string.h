#ifndef _SSE_STRING_H_INCLUDED_
#define _SSE_STRING_H_INCLUDED_

#include <sse_core.h>

#define sse_tolower(c)      ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define sse_toupper(c)      ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

/* string operations */
#define sse_strcpy(d, s)		strcpy((char *) d, (const char *) s)
#define sse_strncpy(d, s, n)	strncpy((char *) d, (const char*) s, (size_t) n)
#define sse_strcat(d, s)		strcat((char *) d, (const char *) s)
#define sse_strncat(d, s, n)	strncat((char *) d, (const char *) s, (size_t) n)
#define sse_strcmp(s1, s2)		strcmp((const char *) s1, (const char *) s2)
#define sse_strncmp(s1, s2, n)	strncmp((const char *) s1, (const char *) s2, (size_t) n)
#define sse_strlen(str)			strlen((const char *) str)

/* memory operations */
#define sse_memcpy(d, s, n)		memcpy(d, s, n)
#define sse_memmove(d, s, n)	memmove(d, s, n)
#define sse_memcmp(s1, s2, n)	memcmp(s1, s2, n)
#define sse_memchr(str, c, n)	memchr(str, c, n)
#define sse_memset(buf, c, n)	memset(buf, c, n)
#define sse_memzero(buf, n)		memset(buf, 0, n)


typedef struct {
	size_t		len;
	char		*data;
} sse_str_t;


#define sse_str_create(str)			{ sse_strlen(str), (char *) str }
#define sse_str_create_null			{ 0, NULL }
#define sse_str_set(str, text)		(str)->len = sse_strlen(text); (str)->data = (char *) text
#define sse_str_set_null(str)		(str)->len = 0; (str)->data = NULL
#define sse_str_assign(str1, str2)	(str1)->len = (str2)->len; (str1)->data = (str2)->data
#define sse_str_copy(str1, str2)	(str1)->len = (str2)->len; sse_memcpy((str1)->data, (str2)->data, (str2)->len)
#define sse_str_is_empty(str)		(str)->len == 0

sse_int_t sse_str_strcmp(sse_str_t *str1, sse_str_t *str2);

#endif /* _SSE_STRING_H_INCLUDED_ */
