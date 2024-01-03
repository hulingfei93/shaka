/*
 * ls_file.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif
static long posix_version = 0;
static long xsi_version = 0;

/* If PATH_MAX is indeterminate, no guarantee this is adequate */
#define PATH_MAX_GUESS 1024

ls_bool_t
ls_file_is_dir(const char *path)
{
	struct stat		stat_buf;

	if (lstat(path, &stat_buf) < 0) {
		return LS_BOOL_FALSE;
	}

	if (S_ISDIR(stat_buf.st_mode)) {
		return LS_BOOL_TRUE;
	}

	return LS_BOOL_FALSE;
}

ls_size_t
ls_file_path_max_length(void)
{
	ls_size_t max_length;

	if (posix_version == 0)
	  posix_version = sysconf(_SC_VERSION);
	if (xsi_version == 0)
	  xsi_version = sysconf(_SC_XOPEN_VERSION);
	
	if (pathmax == 0) { /* first time through */
		errno = 0;
		if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
			if (errno == 0)
				pathmax = PATH_MAX_GUESS; /* it''s indeterminate */
			else
				ls_log_err_exit("ls: pathconf error for _PC_PATH_MAX");
		} 
		else {
			pathmax++; /* add one since it's relative to root */
		}	
	}
	/*
	 * Before POSIX.1-2001, we aren't' guaranteed that PATH_MAX includes
	 * the terminating null byte. Same goes for XPG3.
	 */
	if ((posix_version < 200112L) && (xsi_version < 4))
	  max_length = pathmax + 1;
	else
	  max_length = pathmax;
	
	return max_length;
}

void
ls_options_format_flags_clear(ls_options_t *options)
{
	options->flag_C = LS_BOOL_FALSE;
	options->flag_l = LS_BOOL_FALSE;
	options->flag_1 = LS_BOOL_FALSE;
	options->flag_n = LS_BOOL_FALSE;
	options->flag_x = LS_BOOL_FALSE;
}

void
ls_options_printable_flags_clear(ls_options_t *options)
{
	options->flag_q = LS_BOOL_FALSE;
	options->flag_w = LS_BOOL_FALSE;
}

void
ls_options_time_flags_clear(ls_options_t *options)
{
	options->flag_c = LS_BOOL_FALSE;
	options->flag_u = LS_BOOL_FALSE;
}

void
ls_options_sort_flags_clear(ls_options_t *options)
{
	options->flag_f = LS_BOOL_FALSE;
	options->flag_S = LS_BOOL_FALSE;
	options->flag_t = LS_BOOL_FALSE;
}

void
ls_options_size_display_flags_clear(ls_options_t *options)
{
	options->flag_h = LS_BOOL_FALSE;
	options->flag_k = LS_BOOL_FALSE;
}

