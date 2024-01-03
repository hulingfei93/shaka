/*
 *  sish.c
 */

#include <sish_core.h>

int
main(int argc, char **argv)
{
    sish_int_t          ch;
    sish_options_t      options;
    char                *line_buf;
    sish_size_t         line_buf_size;  
    sish_ssize_t        nread;
    sish_pool_t         *pool;
    sish_vector_t       *commands;
    sish_int_t          exit_code;
    sish_command_t      *command;

    commands = SISH_NULL;
    last_exit_code = 0;

    /* ignore SIGINT signal to avoid ctrl+c termination */
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
        sish_log_err_exit("sish: ignore SIGINT error\n");
    }

    /* set the environment variable "SHELL" to the path of the executable of sish */
    setenv("SHELL", argv[0], 1);

    sish_init_options(&options);
    while ((ch = getopt(argc, argv, "c:x")) != -1) {
        switch(ch) {
            case 'c':
                options.flag_c = true;
                options.command = optarg;
                break;
            case 'x':
                options.flag_x = true;
                break;
            default:
                break;
        }
    }

    if (sish_options_enable_interaction(&options)) {
        line_buf_size = SISH_LINE_BUF_SIZE;
        line_buf = sish_alloc(line_buf_size);
        
        while (1) {
            printf("sish$ ");
            nread = getline(&line_buf, &line_buf_size, stdin);

            if (nread == -1) {
                /* read error */
                if (ferror(stdin) != 0) {
                    sish_log_err_exit("sish: read error\n");
                }
            }
            
            pool = sish_pool_create(SISH_POOL_NORMAL_SIZE);
            exit_code = sish_parse_tokenize(line_buf, &commands, pool);
            if (exit_code != SISH_EXIT_SUCCESS) {
                sish_pool_destory(pool);
                continue;
            }
            exit_code = sish_parse_redirect(commands);
            if (exit_code != SISH_EXIT_SUCCESS) {
                sish_pool_destory(pool);
                continue;
            }
            // sish_parse_debug_info(commands);
            exit_code = sish_process_exec(commands, &options, pool);
            /* save the exit status of last command */
            last_exit_code = exit_code;
            sish_pool_destory(pool);

            /* special process for exit under interaction mode */
            if (commands->size > 0) {
                command = *((sish_command_t **)sish_vector_get(commands, commands->size - 1));
                if (command->is_builtin && strcmp(command->path, "exit") == 0) {
                    exit(exit_code);
                }
            }

        }
        sish_free(line_buf);
    }
    else {
        pool = sish_pool_create(SISH_POOL_NORMAL_SIZE);
        exit_code = sish_parse_tokenize(options.command, &commands, pool);
        if (exit_code != SISH_EXIT_SUCCESS) {
            exit(exit_code);
        }
        exit_code = sish_parse_redirect(commands);
        if (exit_code != SISH_EXIT_SUCCESS) {
            exit(exit_code);
        }
        // sish_parse_debug_info(commands);
        exit_code = sish_process_exec(commands, &options, pool);
        sish_pool_destory(pool);
    }

    exit(exit_code);
}
