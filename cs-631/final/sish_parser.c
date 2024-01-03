/*
 * sish_parser.c
 */

#include <sish_core.h>

#define sish_parse_is_control(c) \
	((c) == '&' || (c) == '|')

#define sish_parse_is_whitespace(c) \
	((c) == ' ' || (c) == '\t')

#define sish_parse_is_redirect(c) \
	((c) == '<' || (c) == '>')

static sish_bool_t sish_parse_is_builtin(char *cmd_name);
static sish_int_t sish_parse_command_add(sish_command_t *command, sish_vector_t *commands, char c, sish_bool_t ignore_empty);
static void sish_parse_token_fetch(sish_command_t *command, sish_vector_t *token_buf, sish_pool_t *pool);
static void sish_parse_token_push(sish_vector_t *token_buf, char *c);


void
sish_init_options(sish_options_t *options)
{
	options->flag_c = false;
	options->command = SISH_NULL;
	options->flag_x = false;
}

/* 
 * a token buffer used for storing the token string temporarily
 * to simplify the tokenization
 */
sish_int_t
sish_parse_tokenize(char *cmd_str, sish_vector_t **commands, sish_pool_t *pool)
{
	char				*cur, *last;
	sish_vector_t		*token_buf;
	sish_command_t		*new_command;

	token_buf = sish_vector_create(sizeof(char), SISH_VECTOR_NORMAL_SIZE, pool);
	*commands = sish_vector_create(sizeof(sish_command_t *), SISH_VECTOR_SMALL_SIZE, pool);

	new_command = (sish_command_t *)sish_pool_alloc(pool, sizeof(sish_command_t));
	new_command->tokens = sish_vector_create(sizeof(char *), SISH_VECTOR_SMALL_SIZE, pool);
	new_command->args = sish_vector_create(sizeof(char *), SISH_VECTOR_SMALL_SIZE, pool);
	new_command->redirect_stdin = SISH_NULL;
	new_command->redirect_stdout = SISH_NULL;
	new_command->redirect_stdout_is_append = false;

	cur = cmd_str;
	while (*cur) {
		if (sish_parse_is_whitespace(*cur)) {
			sish_parse_token_fetch(new_command, token_buf, pool);
		}
		/* end of command */
		else if (sish_parse_is_control(*cur)) {
			sish_parse_token_fetch(new_command, token_buf, pool);
			
			if (sish_parse_command_add(new_command, *commands, *cur, false) == SISH_EXIT_FAILURE) {
				return SISH_EXIT_FAILURE;
			}

			new_command = (sish_command_t *)sish_pool_alloc(pool, sizeof(sish_command_t));
			new_command->tokens = sish_vector_create(sizeof(char *), SISH_VECTOR_SMALL_SIZE, pool);
			new_command->args = sish_vector_create(sizeof(char *), SISH_VECTOR_SMALL_SIZE, pool);
			new_command->redirect_stdin = SISH_NULL;
			new_command->redirect_stdout = SISH_NULL;
			new_command->redirect_stdout_is_append = false;

		}
		else if (*cur == '\n') {
			sish_parse_token_fetch(new_command, token_buf, pool);
		}
		else if (sish_parse_is_redirect(*cur)) {
			if (!sish_vector_is_empty(token_buf)) {
				last = (char *)sish_vector_get(token_buf, token_buf->size - 1);
				if (*last != '>') {
					sish_parse_token_fetch(new_command, token_buf, pool);
				}
			}
			sish_parse_token_push(token_buf, cur);

		}
		else {
			sish_parse_token_push(token_buf, cur);
		}
		++cur;
	}
	sish_parse_token_fetch(new_command, token_buf, pool);
	if (sish_parse_command_add(new_command, *commands, *cur, true) == SISH_EXIT_FAILURE) {
		return SISH_EXIT_FAILURE;
	}

	return SISH_EXIT_SUCCESS;
}

sish_int_t
sish_parse_redirect(sish_vector_t *commands)
{
	sish_uint_t			i, j;
	sish_command_t		*command;
	char				*token, *next_token;
	char				*redirect_input, *redirect_output;
	sish_size_t			token_size;
	char				*termination;

	termination = SISH_NULL;

	for (i = 0; i < commands->size; ++i) {
		command = *((sish_command_t **)sish_vector_get(commands, i));
		redirect_input = redirect_output = SISH_NULL;
		for (j = 0; j < command->tokens->size; ) {
			token = *((char **)sish_vector_get(command->tokens, j));
			token_size = strlen(token);

			/* stdin redirection */
			if (token[0] == '<') {
				if (token_size > 1) {
					redirect_input = token + 1;
					++j;
				}
				/* search next token for stdin redirection*/
				else if (j + 1 < command->tokens->size) {
					next_token = *((char **)sish_vector_get(command->tokens, j + 1));
					if (next_token[0] == '<' || next_token[0] == '>') {
						sish_log_err("sish: syntax error near unexpected token '<'\n");
						return SISH_EXIT_FAILURE;
					}
					else {
						redirect_input = next_token;
					}
					j += 2;
				}
				else {
					sish_log_err("sish: syntax error near unexpected token '<'\n");
					return SISH_EXIT_FAILURE;
				}

				if (command->redirect_stdin != SISH_NULL) {
					sish_log_err("sish: cannot redirect standard input to multiple files\n");
					return SISH_EXIT_FAILURE;
				}
				command->redirect_stdin = redirect_input;

			}
			/* stdout redirection ">>file" */
			else if (strncmp(token, ">>", 2) == 0){
				if (token_size > 2)	{
					redirect_output = token + 2;
					++j;
				}
				/* search next token for stdout redirection */
				else if (j + 1 < command->tokens->size) {
					next_token = *((char **)sish_vector_get(command->tokens, j + 1));
					if (next_token[0] == '<' || next_token[0] == '>') {
						sish_log_err("sish: syntax error near unexpected token '>'\n");
						return SISH_EXIT_FAILURE;
					}
					else {
						redirect_output = next_token;
					}
					j += 2;
				}
				else {
					sish_log_err("sish: syntax error near unexpected token '>'\n");
					return SISH_EXIT_FAILURE;
				}

				if (command->redirect_stdout != SISH_NULL) {
					sish_log_err("sish: cannot redirect standard output to multiple files\n");
					return SISH_EXIT_FAILURE;
				}
				command->redirect_stdout = redirect_output;
				command->redirect_stdout_is_append = true;
			}
			/* stdout redirection ">file" */
			else if (token[0] == '>'){
				if (token_size > 1)	{
					redirect_output = token + 1;
					++j;
				}
				/* search next token for stdout redirection */
				else if (j + 1 < command->tokens->size) {
					next_token = *((char **)sish_vector_get(command->tokens, j + 1));
					if (next_token[0] == '<' || next_token[0] == '>') {
						sish_log_err("sish: syntax error near unexpected token '>'\n");
						return SISH_EXIT_FAILURE;
					}
					else {
						redirect_output = next_token;
					}
					j += 2;
				}
				else {
					sish_log_err("sish: syntax error near unexpected token '>'\n");
					return SISH_EXIT_FAILURE;
				}
				if (command->redirect_stdout != SISH_NULL) {
					sish_log_err("sish: cannot redirect standard output to multiple files\n");
					return SISH_EXIT_FAILURE;
				}
				command->redirect_stdout = redirect_output;
			}
			else {
				sish_vector_push_back(command->args, &token);
				++j;
			}
		} /* end of for */
		if (command->args->size > 0) {
			command->path = *((char **)sish_vector_get(command->args, 0));
		}
		else {
			command->path = SISH_NULL;
		}
		sish_vector_push_back(command->args, &termination);
	} /* end of for */
	return SISH_EXIT_SUCCESS;
}

static sish_bool_t
sish_parse_is_builtin(char *cmd_name) 
{
	return strcmp(cmd_name, "cd") == 0
		|| strcmp(cmd_name, "echo") == 0
		|| strcmp(cmd_name, "exit") == 0;
}

static sish_int_t
sish_parse_command_add(sish_command_t *command, sish_vector_t *commands, 
			char c, sish_bool_t ignore_empty)
{
	char	*cmd_name;

	if (sish_vector_is_empty(command->tokens)) {
		if (!ignore_empty) {
			sish_log_err("sish: syntax error near unexpected token '%c'\n", c);
			return SISH_EXIT_FAILURE;
		}
	}
	/* add command into the vector, then begin an new command */
	else {
		cmd_name = *((char **)sish_vector_get(command->tokens, 0));
		if (sish_parse_is_builtin(cmd_name)) {
			command->is_builtin = true;
		}
		else {
			command->is_builtin = false;
		}

		if (c == '|') {
			command->is_pipeline = true;
			command->is_background = false;
		}
		else if (c == '&') {
			command->is_pipeline = false;
			command->is_background = true;
		}
		else {
			command->is_pipeline = false;
			command->is_background = false;
		}
		sish_vector_push_back(commands, &command);
	}
	return SISH_EXIT_SUCCESS;
}

/* add the token from buffer to command token vector */
static void
sish_parse_token_fetch(sish_command_t *command, sish_vector_t *token_buf, sish_pool_t *pool)
{
	char	*new_token;

	if (!sish_vector_is_empty(token_buf)) {
		new_token = sish_pool_calloc(pool, token_buf->size + 1);
		memcpy(new_token, token_buf->data, token_buf->size);
		sish_vector_push_back(command->tokens, &new_token);
		sish_vector_cleanup(token_buf);
	}
}

static void
sish_parse_token_push(sish_vector_t *token_buf, char *c)
{
	sish_vector_push_back(token_buf, c);
}

void
sish_parse_debug_info(sish_vector_t *commands)
{
	sish_uint_t			command_count, i, j;
	sish_command_t		*command;
	char				*token, *arg;

	command_count = commands->size;
	printf("total: %d commands\n", command_count);

	for (i = 0; i < command_count; ++i) {
		printf ("command: %d\n", i + 1);
		command = *((sish_command_t **)sish_vector_get(commands, i));
		printf("builtin: %d, pipeline: %d, background: %d\n",
					command->is_builtin, command->is_pipeline, command->is_background);

		printf("tokens:");
		for (j = 0; j < command->tokens->size; ++j) {
			token = *((char **)sish_vector_get(command->tokens, j));
			printf(" %s", token);
		}
		printf("\n");

		printf("stdin: %s, stdout: %s, append: %d\n", command->redirect_stdin,
					command->redirect_stdout, command->redirect_stdout_is_append);

		printf("args:");
		for (j = 0; j < command->args->size; ++j) {
			arg = *((char **)sish_vector_get(command->args, j));
			printf(" %s", arg);
		}
		printf("\n\n");

	}
}
