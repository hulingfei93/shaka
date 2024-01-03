/*
 * sish_process.c
 */

#include <sish_core.h>

static void sish_process_fd_dup(sish_fd_t fd, sish_fd_t new_fd);
static sish_int_t sish_process_change_cwd(sish_command_t *command);
static void sish_process_echo(sish_command_t *command);
static void sish_process_print_tracing(sish_command_t *command);

sish_int_t
sish_process_exec(sish_vector_t *commands, sish_options_t *options, sish_pool_t *pool)
{
	sish_uint_t			i;
	sish_command_t		*command;
	sish_fd_t			pipe_fd[2];
	sish_fd_t			save_fd;	/* save the read end of pipe */
	sish_pid_t			pid; 
	sish_vector_t		*pids;
	sish_pid_t			*pid_ptr;
	sish_int_t			exit_code, exit_status, return_code;

	save_fd = -1;
	pids = sish_vector_create(sizeof(sish_pid_t), SISH_VECTOR_SMALL_SIZE, pool);

	for (i = 0; i < commands->size; ++i)
	{
		command = *((sish_command_t **)sish_vector_get(commands, i));

		if (command->path == SISH_NULL) {
			continue;
		}

		if (sish_options_enable_tracing(options)) {
			sish_process_print_tracing(command);
		}
		
		if (command->is_builtin) {
			if (strcmp(command->path, "cd") == 0) {
				sish_process_change_cwd(command);
			}
		}

		if (pipe(pipe_fd) < 0) {
			sish_log_err("sish: pipe error\n");
			return SISH_EXIT_FAILURE;
		}

		if ((pid = fork()) < 0) {
			sish_log_err("sish: fork error\n");
			return SISH_EXIT_FAILURE;
		}
		/* parent process */
		else if (pid > 0) {
			close(pipe_fd[1]);
			if (save_fd != -1) {
				close(save_fd);
			}
			save_fd = pipe_fd[0];
			if (!command->is_background) {
				sish_vector_push_back(pids, &pid);
			}
		}
		/* child process */
		else {
			sish_fd_t		input_fd, output_fd;

			close(pipe_fd[0]);
			
			/* pipeline */
			if (i < commands->size - 1 && !command->is_background) {
				sish_process_fd_dup(pipe_fd[1], SISH_FD_STDOUT);
			}

			if (i > 0) {
				sish_process_fd_dup(save_fd, SISH_FD_STDIN);
			}

			/* redirection */
			if (command->redirect_stdin != SISH_NULL) {
				input_fd = open(command->redirect_stdin, O_RDONLY);
				if (input_fd == -1) {
					sish_log_err_exit("sish: open %s: %s\n", command->redirect_stdin, 
								strerror(errno));
				}
				sish_process_fd_dup(input_fd, SISH_FD_STDIN);
			}
			if (command->redirect_stdout != SISH_NULL) {
				if (command->redirect_stdout_is_append) {
					output_fd = open(command->redirect_stdout, 
								O_WRONLY | O_CREAT | O_APPEND, SISH_FILE_MODE);
				}
				else {
					output_fd = open(command->redirect_stdout, 
								O_WRONLY | O_CREAT, SISH_FILE_MODE);
				}
				if (output_fd == -1) {
					sish_log_err_exit("sish: open %s: %s\n", command->redirect_stdout, 
								strerror(errno));
				}
				sish_process_fd_dup(output_fd, SISH_FD_STDOUT);
			}
		
			/* builtins */
			if (command->is_builtin) {
				if (strcmp(command->path, "cd") == 0) {
					return_code = sish_process_change_cwd(command);
					if (return_code == SISH_EXIT_FAILURE) {
						fprintf(stderr, "cd %s: \n", strerror(errno));
					}
					exit(return_code);
				}
				else if (strcmp(command->path, "exit") == 0) {
					exit(SISH_EXIT_SUCCESS);
				}
				else {
					sish_process_echo(command);
					exit(SISH_EXIT_SUCCESS);
				}
			}
			/* external commands */
			else {
				if (execvp(command->path, (char **)(command->args->data)) < 0) {
					sish_log_err_exit("sish: %s: %s\n", command->path, strerror(errno));
				}
			}
		} /* end of child */
	} /* end of for */

	exit_code = 0;
	/* wait for child processes */
	for (i = 0; i < pids->size; ++i) {
		pid_ptr = (sish_pid_t *)sish_vector_get(pids, i);
		if (waitpid(*pid_ptr, &exit_status, 0) != *pid_ptr) {
			sish_log_err("sish: waitpid: %s\n", strerror(errno));
			return SISH_EXIT_FAILURE;
		}
		else {
			if (WIFEXITED(exit_status)) {
				exit_code = WEXITSTATUS(exit_status);
			}
			else {
				exit_code = SISH_EXIT_FAILURE;
			}
		}
	}
	return exit_code;
}

static void
sish_process_print_tracing(sish_command_t *command)
{
	sish_uint_t		i;
	char			*token;
	fprintf(stderr, "+ ");
	for (i = 0; i < command->tokens->size; ++i) {
		token = *((char **)sish_vector_get(command->tokens, i));
		fprintf(stderr, "%s ", token);
	}
	fprintf(stderr, "\n");
}

static void
sish_process_echo(sish_command_t *command)
{
	sish_uint_t		i;
	char			*arg;
	for (i = 1; i < command->args->size - 1; ++i) {
		arg = *((char **)sish_vector_get(command->args, i));
		if (strcmp(arg, "$$") == 0) {
			printf("%d ", getpid());
		}
		else if(strcmp(arg, "$?") == 0) {
			printf("%d ", last_exit_code);
		}
		else {
			printf("%s ", arg);
		}
	}
	printf("\n");
	fflush(stdout);
}

static sish_int_t
sish_process_change_cwd(sish_command_t *command)
{
	char			*cwd_path;
	struct passwd	*pwd;

	cwd_path = SISH_NULL;
	if (command->args->size > 2) {
		cwd_path = *((char **)sish_vector_get(command->args, 1));
	}
	else {
		pwd = getpwuid(getuid());
		if (pwd != SISH_NULL) {
			cwd_path = pwd->pw_dir;
		}
	}

	if (cwd_path == SISH_NULL) {
		return SISH_EXIT_FAILURE;
	}

	if (chdir(cwd_path) < 0) {
		return SISH_EXIT_FAILURE;
	}

	return SISH_EXIT_SUCCESS;
}

static void
sish_process_fd_dup(sish_fd_t fd, sish_fd_t new_fd)
{
	if (fd != new_fd) {
		dup2(fd, new_fd);
		close(fd);
	}
}

