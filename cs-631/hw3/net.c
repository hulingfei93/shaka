/*
 * net.c
 * Copyright (C) Lingfei Hu
 */

#include <core.h>

static void *request_func(void *arg);
static void parse_addr(options_t *options);
static bool is_valid_ipv4(const char *addr_str, struct sockaddr_in *sa);
static bool is_valid_ipv6(const char *addr_str, struct sockaddr_in6 *sa);

void
init_options(options_t *options)
{
    options->is_allow_cgi = false;
    options->cgi_root_dir = NULL;
    options->is_debug_mode = false;
    options->is_print_exit = false;
    options->addr = NULL;
    options->is_ipv4 = false;
    options->is_ipv6 = false;
    options->is_log = false;
    options->log_path = NULL;
    options->port = "8080";
    options->doc_root = NULL;
}

fd_t 
init_server(options_t *options)
{
    fd_t    listen_socket;

    /* create socket for listen */
    if (options->addr != NULL) {
        parse_addr(options);
        if (options->is_ipv4) {
            listen_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (listen_socket < 0) {
                err_exit("opening stream socket: %s\n", strerror(errno));
            }
            options->sa.sin_family = AF_INET;
            options->sa.sin_port = htons(atoi(options->port));
            if (bind(listen_socket, (struct sockaddr *)(&options->sa), sizeof(options->sa)) < 0) {
                err_exit("binding stream socket: %s\n", strerror(errno));
            }
        }
        else if (options->is_ipv6) {
            listen_socket = socket(AF_INET6, SOCK_STREAM, 0);
            if (listen_socket < 0) {
                err_exit("opening stream socket: %s\n", strerror(errno));
            }
            options->sa.sin_family = AF_INET6;
            options->sa.sin_port = htons(atoi(options->port));
            if (bind(listen_socket, (struct sockaddr *)(&options->sa6), sizeof(options->sa6)) < 0) {
                err_exit("binding stream socket: %s\n", strerror(errno));
            }
        }
        else {
            err_exit("%s is not a valid ipv4 or ipv6 address", options->addr);
        }
        if (listen(listen_socket, CONN_REQUESTS_QUEUE_LEN) < 0) {
            err_exit("listening stream socket: %s\n", strerror(errno));
        }
    }
    /* listen all addresses */
    else {
        options->is_ipv4 = true;
        listen_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_socket < 0) {
            err_exit("opening stream socket: %s\n", strerror(errno));
        }
        
        options->sa.sin_family = AF_INET;
        options->sa.sin_addr.s_addr = htonl(INADDR_ANY);
        options->sa.sin_port = htons(atoi(options->port));
        if (bind(listen_socket, (struct sockaddr *)(&options->sa), sizeof(options->sa)) < 0) {
            err_exit("binding stream socket: %s\n", strerror(errno));
        }
        
        if (listen(listen_socket, CONN_REQUESTS_QUEUE_LEN) < 0) {
            err_exit("listening stream socket: %s\n", strerror(errno));
        }
    }

    return listen_socket;
}

void
start_server(fd_t listen_socket, options_t *options)
{
    fd_t                request_socket;
    struct sockaddr     *client;
    socklen_t           client_len;
    request_t           *new_request;
    pthread_t           request_thread;

    if (options->is_ipv4) {
        client_len = sizeof(struct sockaddr_in);
        client = (struct sockaddr *)malloc(client_len);
    }
    else {
        client_len = sizeof(struct sockaddr_in6);
        client = (struct sockaddr *)malloc(client_len);
    }

    if (client == NULL) {
        err_exit("malloc error\n");
    }

    while (true) {
        request_socket = accept(listen_socket, client, &client_len);
        if (request_socket == -1) {
            err("accept error: %s\n", strerror(errno));
        }
        /* create a new thread to handle */
        else {
            new_request = (request_t *)malloc(sizeof(request_t));
            if (client == NULL) {
                err("malloc error\n");
            }
            else {
                new_request->socket = request_socket;
                new_request->is_ipv4 = options->is_ipv4;
                if (options->is_ipv4) {
                    new_request->client = (struct sockaddr *)malloc(client_len);
                    memcpy(new_request->client, client, client_len);
                }
                else {
                    new_request->client = (struct sockaddr *)malloc(client_len);
                    memcpy(new_request->client, client, client_len);
                }
                
                if (pthread_create(&request_thread, NULL, request_func, new_request) != 0) {
                    err("pthread_create error: %s\n", strerror(errno));
                }
                if (pthread_detach(request_thread) != 0) {
                    err("pthread_detach error: %s\n", strerror(errno));
                }

            }
        }
    }

    free(client);
}

static void *
request_func(void *arg)
{
    int nread;
    request_t *request;
    
    request = (request_t *)arg;
    
    if (request->is_ipv4) {
        struct sockaddr_in *sa = (struct sockaddr_in *)(request->client);
        inet_ntop(AF_INET, &(sa->sin_addr), request->buf, BUF_SIZE);
        printf("%s:%d: ", request->buf, ntohs(sa->sin_port));
        fflush(stdout); 
    }
    else {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)(request->client);
        inet_ntop(AF_INET6, &(sa6->sin6_addr), request->buf, BUF_SIZE);
        printf("%s:%d: ", request->buf, ntohs(sa6->sin6_port));
        fflush(stdout); 
    }

    while ((nread = read(request->socket, request->buf, BUF_SIZE - 1)) > 0) {
        request->buf[nread] = '\0';
        printf("%s", request->buf);
        fflush(stdout); 
    }

    if (nread < 0) {
        err("read error: %s\n", strerror(errno));
    }
    
    close(request->socket);
    printf("Connection closed.\n");
    free(request->client);
    free(request);
    
    return NULL;
}


static void
parse_addr(options_t *options) 
{

    options->is_ipv4 = is_valid_ipv4(options->addr, &(options->sa));
    options->is_ipv6 = is_valid_ipv6(options->addr, &(options->sa6));
}

static bool
is_valid_ipv4(const char *addr_str, struct sockaddr_in *sa)
{
    return inet_pton(AF_INET, addr_str, &(sa->sin_addr)) == 1 ? true : false;
}

static bool
is_valid_ipv6(const char *addr_str, struct sockaddr_in6 *sa)
{
    return inet_pton(AF_INET6, addr_str, &(sa->sin6_addr)) == 1 ? true : false;
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

void
err(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
