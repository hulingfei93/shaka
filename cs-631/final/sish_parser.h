/*
 * sish_parser.h
 */

#ifndef _SISH_PARSER_H_INCLUDED_
#define _SISH_PARSER_H_INCLUDED_

#include <sish_core.h>

#define sish_options_enable_interaction(options) \
    (!(options)->flag_c)

#define sish_options_enable_tracing(options) \
    ((options)->flag_x)

typedef struct {
    sish_bool_t     flag_c;
    char            *command;
    sish_bool_t     flag_x;
} sish_options_t;

typedef struct {
    sish_vector_t   *tokens;
    sish_vector_t   *args;
    char            *path;
    char            *redirect_stdin;
    char            *redirect_stdout;
    sish_bool_t     redirect_stdout_is_append;
    sish_bool_t     is_builtin;
    sish_bool_t     is_pipeline;
    sish_bool_t     is_background;
} sish_command_t;


void sish_init_options(sish_options_t *options);
sish_int_t sish_parse_tokenize(char *cmd_str, sish_vector_t **commands, sish_pool_t *pool);
sish_int_t sish_parse_redirect(sish_vector_t *commands);
void sish_parse_debug_info(sish_vector_t *commands);

#endif /* _SISH_PARSER_H_INCLUDED_ */
