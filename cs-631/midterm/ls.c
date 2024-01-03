/*
 * ls.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

static void usage(void);

int
main(int argc, char **argv) 
{
    int             ch, i, operand_count;
    char            *path, *cwd;
    ls_options_t    options;
    ls_pool_t       *pool;
    ls_vector_t     *file_paths, *dir_paths;
    struct stat     stat_buf;

    setprogname(argv[0]);

    /* set options */
    ls_memset(&options, LS_BOOL_FALSE, sizeof(ls_options_t));
    while ((ch = getopt(argc, argv, "AaCcdFfhiklnqRrSstuwxl")) != -1) {
        switch(ch) {
            case 'A':
                options.flag_A = LS_BOOL_TRUE;
                break;
            case 'a':
                options.flag_a = LS_BOOL_TRUE;
                break;
            case 'C':
                ls_options_format_flags_clear(&options);
                options.flag_C = LS_BOOL_TRUE;
                break;
            case 'c':
                ls_options_time_flags_clear(&options);
                options.flag_c = LS_BOOL_TRUE;
                break;
            case 'd':
                options.flag_d = LS_BOOL_TRUE;
                break;
            case 'F':
                options.flag_F = LS_BOOL_TRUE;
                break;
            case 'f':
                ls_options_sort_flags_clear(&options);
                options.flag_f = LS_BOOL_TRUE;
                break;
            case 'h':
                ls_options_size_display_flags_clear(&options);
                options.flag_h = LS_BOOL_TRUE;
                break;
            case 'i':
                options.flag_i = LS_BOOL_TRUE;
                break;
            case 'k':
                ls_options_size_display_flags_clear(&options);
                options.flag_k = LS_BOOL_TRUE;
                break;
            case 'l':
                ls_options_format_flags_clear(&options);
                options.flag_l = LS_BOOL_TRUE;
                break;
            case 'n':
                ls_options_format_flags_clear(&options);
                options.flag_n = LS_BOOL_TRUE;
                break;
            case 'q':
                ls_options_printable_flags_clear(&options);
                options.flag_q = LS_BOOL_TRUE;
                break;
            case 'R':
                options.flag_R = LS_BOOL_TRUE;
                break;
            case 'r':
                options.flag_r = LS_BOOL_TRUE;
                break;
            case 'S':
                ls_options_sort_flags_clear(&options);
                options.flag_S = LS_BOOL_TRUE;
                break;
            case 's':
                options.flag_s = LS_BOOL_TRUE;
                break;
            case 't':
                ls_options_sort_flags_clear(&options);
                options.flag_t = LS_BOOL_TRUE;
                break;
            case 'u':
                ls_options_time_flags_clear(&options);
                options.flag_u = LS_BOOL_TRUE;
                break;
            case 'w':
                ls_options_printable_flags_clear(&options);
                options.flag_w = LS_BOOL_TRUE;
                break;
            case 'x':
                ls_options_format_flags_clear(&options);
                options.flag_x = LS_BOOL_TRUE;
                break;
            case '1':
                ls_options_format_flags_clear(&options);
                options.flag_1 = LS_BOOL_TRUE;
                break;
            default:
                exit(LS_EXIT_FAILURE);
                break;
        }
    }
    options.flag_terminal = isatty(SSE_FD_STDOUT);


    pool = ls_pool_create(LS_POOL_SMALL_SIZE);

    file_paths = ls_vector_create(sizeof(char *), LS_VECTOR_NORMAL_SIZE, pool);
    dir_paths = ls_vector_create(sizeof(char *), LS_VECTOR_NORMAL_SIZE, pool);

    operand_count = 0;
    for (i = 1; i < argc; ++i) {
        path = argv[i];
        if (path[0] == '-') {
            continue;
        }
        
        ++operand_count;

        if (lstat(path, &stat_buf) < 0) {
            ls_log_err("ls: cannot access %s: %s\n", path, strerror(errno));
        }

        if (!ls_options_is_dir_no_search(&options)
                    && S_ISDIR(stat_buf.st_mode)) {
            ls_vector_push_back(dir_paths, &path);
        }
        else {
            ls_vector_push_back(file_paths, &path);
        }
    }

    /* If no operands are given, the contents of the current directory are displayed */
    if (operand_count == 0) {
        cwd = ls_pool_alloc(pool, ls_strlen(LS_CUR_DIR) + 1);
        ls_strcpy(cwd, LS_CUR_DIR);
        ls_vector_push_back(dir_paths, &cwd);
    }

    ls_print_init(pool);
    ls_process_files(file_paths, &options); 
    ls_process_dirs(dir_paths, &options);

    /* clean up */
    ls_pool_destory(pool);

    exit(LS_EXIT_SUCCESS);
}

static void
usage(void)
{
    printf("Usage: ls [-AacCdFfhiklnqRrSstuwx1] [file ...]\n");
    printf("List directory contents.\n\n");
}


