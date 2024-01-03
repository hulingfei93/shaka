/*
 * net.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _NET_H_INCLUDED_
#define _NET_H_INCLUDED_

#include <core.h>

typedef struct {
    bool                    is_allow_cgi;
    char                    *cgi_root_dir;
    bool                    is_debug_mode;
    bool                    is_print_exit;
    char                    *addr;
    bool                    is_ipv4;
    struct sockaddr_in      sa;
    bool                    is_ipv6;
    struct sockaddr_in6     sa6;
    bool                    is_log;
    char                    *log_path;
    char                    *port;
    char                    *doc_root;
} options_t;

typedef struct {
    fd_t                    socket;
    bool                    is_ipv4;
    struct sockaddr         *client;
    char                    buf[BUF_SIZE];
} request_t;

void err_exit(const char *fmt, ...);
void err(const char *fmt, ...);

void init_options(options_t *options);
fd_t init_server(options_t *options);
void start_server(fd_t listen_socket, options_t *options);

#endif /* _NET_H_INCLUDED_ */
