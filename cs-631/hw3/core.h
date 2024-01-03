/*
 * core.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <string.h>
#include <unistd.h>

#define false	0
#define true	1

#define CONN_REQUESTS_QUEUE_LEN 10
#define BUF_SIZE 1024


typedef int		fd_t;

typedef unsigned char bool;

#include <net.h>

#endif /* _CORE_H_INCLUDED_ */
