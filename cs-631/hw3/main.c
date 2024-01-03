/*
 * main.c
 * Copyright (C) Lingfei Hu
 */

#include <core.h>

static void usage(void);

int
main(int argc, char **argv)
{
    int         ch, i;
    options_t   options;
    char        *arg;
    bool        is_option;

    setprogname(argv[0]);

    init_options(&options);
    /* set options */
    while ((ch = getopt(argc, argv, "c:dhi:l:p:")) != -1) {
        switch(ch) {
            case 'c':
                options.is_allow_cgi = true;
                options.cgi_root_dir = optarg;
                break;
            case 'd':
                options.is_debug_mode = true;
                break;
            case 'h':
                options.is_print_exit = true;
                break;
            case 'i':
                options.addr = optarg;
                break;
            case 'l':
                options.is_log = true;
                options.log_path = optarg;
                break;
            case 'p':
                options.port = optarg;
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (options.is_print_exit) {
        usage();
        exit(EXIT_SUCCESS);
    }

    is_option = false;
    for (i = 1; i < argc; ++i) {
        arg = argv[i];
        if (arg[0] == '-') {
            is_option = true;
        }
        else {
            if (is_option) {
                is_option = false;
            }
            else {
                options.doc_root = arg;
            }
        }
    }
    
    if (options.doc_root == NULL) {
        usage();
        exit(EXIT_FAILURE);
    }

    fd_t listen_socket = init_server(&options);
    start_server(listen_socket, &options);

    exit(EXIT_SUCCESS);
}

static void
usage(void)
{
    printf("Usage: sws [-dh] [-c dir] [-i address] [-l file] [-p port] dir\n");
}
