/*
 * ls_print.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

static char * ls_print_get_path_name(char *file_path, const char *dir_path, ls_mode_t mode, ls_options_t *options, ls_pool_t *pool);
static char ls_print_get_append_indicator(ls_mode_t mode);
static char * ls_print_path_combine(char *path1, char c, ls_pool_t *pool);
static char * ls_print_path_combine2(char *path1, char *path2, char *path3, ls_pool_t *pool);
static char * ls_print_path_combine3(char *path1, char *path2, char *path3, char c, ls_pool_t *pool);
static char *ls_print_path_combine4(const char *dir, const char *name, ls_pool_t *pool);
static void ls_print_long_imp(ls_vector_t *file_info_vector, ls_options_t *options, ls_pool_t *pool);
static void ls_print_imp(ls_vector_t *file_info_vector, ls_options_t *options, ls_pool_t *pool);
static void ls_print_preprocess(ls_vector_t *file_info_vector, const char *dir_path, ls_options_t *options, ls_pool_t *pool);
static void ls_print_size_proc(ls_file_info_t *file_info, ls_options_t *options);
static ls_size_t ls_print_integer_size(ls_uint64_t number);
static ls_size_t ls_print_double_size(ls_double_t number);
static void ls_print_total_size(ls_vector_t *file_info_vector, ls_options_t *options);

static char indicator[] = {'/', '*', '@', '%', '=', '|'};
static char	*max_path_buf;
static int	default_block_size = 512;
static int	block_size = 512;

void ls_print_init(ls_pool_t *pool)
{
	char *block_str;

	max_path_buf = ls_pool_alloc(pool, ls_file_path_max_length());

	block_str = getenv("BLOCKSIZE");
	if (block_str != LS_NULL) {
		block_size = atoi(block_str);
		if (block_size <= 0) {
			block_size = default_block_size;
		}
	}
}

void
ls_print(ls_vector_t *file_info_vector, const char *dir_path, 
			ls_options_t *options, ls_pool_t *pool)
{

	ls_print_preprocess(file_info_vector, dir_path, options, pool);
	
	if (dir_path != LS_NULL) {
		printf("%s:\n", dir_path);
		ls_print_total_size(file_info_vector, options);
	}

	if (ls_options_is_long_format(options)) {
		ls_print_long_imp(file_info_vector, options, pool);
	}
	else {
		ls_print_imp(file_info_vector, options, pool);
	}

	if (dir_path != LS_NULL) {
		printf("\n");
	}
}

static void
ls_print_total_size(ls_vector_t *file_info_vector, ls_options_t *options)
{
	ls_uint_t			i;
	ls_file_info_t		*file_info;
	ls_uint64_t			total_size;
	
	total_size = 0;
	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);

		total_size += file_info->block_number;
	}

	total_size = ceil(total_size * ((ls_double_t)default_block_size / block_size));

	printf("total %llu\n", total_size);
}

static void
ls_print_long_imp(ls_vector_t *file_info_vector, ls_options_t *options, ls_pool_t *pool)
{
	char				mode_str[12];
	char				format_buf[LS_FORMAT_BUF_SIZE];
	char				data_buf[LS_DATA_BUF_SIZE];
	int					data_start;
	ls_print_info_t		*print_info;
	ls_uint_t			i;
	ls_file_info_t		*file_info;
	struct tm			*time;

	print_info = (ls_print_info_t *)ls_pool_alloc(pool, sizeof(ls_print_info_t));
	
	print_info->max_inode = 0;
	print_info->max_block_number = 0;
	print_info->max_converted_block_number = 0;
	print_info->max_mode_size = 10;
	print_info->max_link_number = 0;
	print_info->max_owner_id = 0;
	print_info->max_owner_size = 0;
	print_info->max_group_id = 0;
	print_info->max_group_size = 0;
	print_info->max_size = 0;
	print_info->max_readable_size = 0.0;
	print_info->max_month_size = 3;
	print_info->max_day = 0;
	print_info->max_hour_min_size = 5;
	print_info->max_pathname_size = 0;


	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);
		
		print_info->max_inode = ls_max(print_info->max_inode, file_info->inode_number);
		print_info->max_block_number = ls_max(print_info->max_block_number, file_info->block_number);
		print_info->max_converted_block_number = ls_max(
					print_info->max_converted_block_number, file_info->converted_block_number);

		strmode(file_info->mode, mode_str);
		print_info->max_mode_size = ls_max(print_info->max_mode_size, ls_strlen(mode_str));

		print_info->max_link_number = ls_max(print_info->max_link_number, file_info->link_number);
		
		print_info->max_owner_id = ls_max(print_info->max_owner_id, file_info->owner_id);
		print_info->max_group_id = ls_max(print_info->max_group_id, file_info->group_id);
		if (ls_options_is_print_owner_group_name(options)) {
			if (file_info->owner_name != LS_NULL) {
				print_info->max_owner_size = ls_max(print_info->max_owner_size, ls_strlen(file_info->owner_name));
			}
			else {
				print_info->max_owner_size = ls_max(print_info->max_owner_size, ls_print_integer_size((ls_uint64_t)(file_info->owner_id)));
			}
			if (file_info->group_name != LS_NULL) {
				print_info->max_group_size = ls_max(print_info->max_group_size, ls_strlen(file_info->group_name));
			}
			else {
				print_info->max_group_size = ls_max(print_info->max_group_size, ls_print_integer_size((ls_uint64_t)(file_info->group_id)));
			}
		}
		
		print_info->max_size = ls_max(print_info->max_size, file_info->size);
		
		if(ls_options_is_print_size_readable(options)) {
			print_info->max_readable_size = ls_max(print_info->max_readable_size, file_info->readable_size);
		}

		time = localtime(&file_info->time.tv_sec);
		print_info->max_day = ls_max(print_info->max_day, (ls_uint_t)(time->tm_mday));
		
		print_info->max_pathname_size = ls_max(print_info->max_pathname_size, ls_strlen(file_info->path_name));

	}

	print_info->max_inode_size = ls_print_integer_size((ls_uint64_t)(print_info->max_inode));
	print_info->max_block_number_size = ls_print_integer_size((ls_uint64_t)(print_info->max_block_number));
	print_info->max_converted_block_number_size = ls_print_integer_size((ls_uint64_t)(print_info->max_converted_block_number));
	print_info->max_link_number_size = ls_print_integer_size((ls_uint64_t)(print_info->max_link_number));
	print_info->max_owner_id_size = ls_print_integer_size((ls_uint64_t)(print_info->max_owner_id));
	print_info->max_group_id_size = ls_print_integer_size((ls_uint64_t)(print_info->max_group_id));
	print_info->max_size_size = ls_print_integer_size((ls_uint64_t)(print_info->max_size));

	print_info->max_readable_size_size = ls_print_integer_size((ls_uint64_t)(print_info->max_readable_size));
	print_info->max_readable_size_size += 3; 

	print_info->max_day_size = ls_print_integer_size((ls_uint64_t)(print_info->max_day));

	/* print out */
	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);

		
		data_start = 0;
		if (ls_options_is_print_inode_number(options)) {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_inode_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->inode_number);

		}
		if (ls_options_is_print_block_number(options)) {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_converted_block_number_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->converted_block_number);
		}


		/* mode */
		snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%-%lus ", print_info->max_mode_size);
		strmode(file_info->mode, mode_str);
		data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, mode_str);

		/* link number */
		snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_link_number_size);
		data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->link_number);

		/* user group */
		if (ls_options_is_print_owner_group_name(options)) {
			if (file_info->owner_name != LS_NULL) {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%-%lus ", print_info->max_owner_size);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->owner_name);
			}
			else {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%-%lud ", print_info->max_owner_size);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->owner_id);
			}
			if (file_info->group_name != LS_NULL) {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%-%lus ", print_info->max_group_size);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->group_name);
			}
			else {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%-%lud ", print_info->max_group_size);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->group_id);
			}
		}
		else {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_owner_id_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->owner_id);
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_group_id_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->group_id);
		}
		
		/* size */
		if(ls_options_is_print_size_readable(options)) {
			if (file_info->size_unit != '\0') {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lu.1f%%c ", print_info->max_readable_size_size - 1);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->readable_size, file_info->size_unit);			
			}
			else {
				snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lu.1f ", print_info->max_readable_size_size);
				data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->readable_size);			
			}
		}
		else {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_size_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->size);
		}

		/* time */
		time = localtime(&file_info->time.tv_sec);
		data_start += strftime(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, "%b ", time);

		snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_day_size);
		data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, time->tm_mday);

		data_start += strftime(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, "%R ", time);


		/* path name */
		data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, "%-s", file_info->path_name);	

		printf("%s\n", data_buf);
	}

}

static void
ls_print_imp(ls_vector_t *file_info_vector, ls_options_t *options, ls_pool_t *pool)
{
	char				format_buf[LS_FORMAT_BUF_SIZE];
	char				data_buf[LS_DATA_BUF_SIZE];
	int					data_start;
	ls_print_info_t		*print_info;
	ls_uint_t			i;
	ls_file_info_t		*file_info;

	print_info = (ls_print_info_t *)ls_pool_alloc(pool, sizeof(ls_print_info_t));
	
	print_info->max_inode = 0;
	print_info->max_block_number = 0;
	print_info->max_converted_block_number = 0;
	print_info->max_pathname_size = 0;


	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);
		
		print_info->max_inode = ls_max(print_info->max_inode, file_info->inode_number);
		print_info->max_block_number = ls_max(print_info->max_block_number, file_info->block_number);
		print_info->max_converted_block_number = ls_max(
					print_info->max_converted_block_number, file_info->converted_block_number);

		print_info->max_pathname_size = ls_max(print_info->max_pathname_size, ls_strlen(file_info->path_name));

	}

	print_info->max_inode_size = ls_print_integer_size((ls_uint64_t)(print_info->max_inode));
	print_info->max_block_number_size = ls_print_integer_size((ls_uint64_t)(print_info->max_block_number));
	print_info->max_converted_block_number_size = ls_print_integer_size((ls_uint64_t)(print_info->max_converted_block_number));

	/* print out */
	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);

		
		data_start = 0;
		if (ls_options_is_print_inode_number(options)) {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_inode_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->inode_number);

		}
		if (ls_options_is_print_block_number(options)) {
			snprintf(format_buf, LS_FORMAT_BUF_SIZE, "%%%lud ", print_info->max_converted_block_number_size);
			data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, format_buf, file_info->converted_block_number);
		}

		/* path name */
		data_start += snprintf(data_buf + data_start, LS_DATA_BUF_SIZE - data_start, "%-s", file_info->path_name);	

		printf("%s\n", data_buf);
	}
}

static ls_size_t
ls_print_integer_size(ls_uint64_t number)
{
	ls_size_t size;

	size = 1;

	if (number == 0) {
		return 1;
	}

	while (number /= 10) {
		++size;
	}

	return size;
}

static ls_size_t
ls_print_double_size(ls_double_t number)
{
	ls_size_t size;

	size = 1;

	if (number == 0.0) {
		return 1;
	}

	while ((number /= 10) < 1.0) {
		++size;
	}

	return size;
}

static void
ls_print_preprocess(ls_vector_t *file_info_vector, const char *dir_path, 
			ls_options_t *options, ls_pool_t *pool)
{
	ls_uint_t		i;
	ls_file_info_t	*file_info;
	char			*c;
	struct passwd	*pwd;
	struct group	*grp;
	
	for (i = 0; i < file_info_vector->size; ++i) {
		file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);
		
		/* -F */
		file_info->path_name = ls_print_get_path_name(
					file_info->path_name, dir_path, file_info->mode, options, pool);

		/* -k -h */
		ls_print_size_proc(file_info, options);
		
		/* -q -w */
		if (ls_options_is_print_replace_nonprintable(options)) {
			for (c = file_info->path_name; *c != '\0'; ++c) {
				if (!isprint(*c)) {
					*c = '?';
				}
			}
		}
		/* -s */
		if (ls_options_is_print_block_number(options)) {
			if (default_block_size != block_size) {
				file_info->converted_block_number 
					= (ls_blkcnt_t)ceil(file_info->block_number * ((ls_double_t)default_block_size / (ls_double_t)block_size));
			}
			else {
				file_info->converted_block_number= file_info->block_number;
			}
		}

		/* -l -n */
		if (ls_options_is_print_owner_group_name(options)) {
			pwd = getpwuid(file_info->owner_id);
			if (pwd != LS_NULL) {
				file_info->owner_name = pwd->pw_name;	
			}
			else {
				file_info->owner_name = LS_NULL;
			}

			grp = getgrgid(file_info->group_id);
			if (grp != LS_NULL) {
				file_info->group_name = grp->gr_name;
			}
			else {
				file_info->group_name = LS_NULL;
			}
		}

	}
}

static void
ls_print_size_proc(ls_file_info_t *file_info, ls_options_t *options)
{
	if (ls_options_is_print_size_kb(options)) {
		file_info->size = (ls_off_t)ceil((ls_double_t)(file_info->size) / 1024);
		file_info->size_unit = 'K';
	}
	else if(ls_options_is_print_size_readable(options)) {
		if (file_info->size >= 1073741824) {
			file_info->readable_size = (ls_double_t)(file_info->size) / 1073741824;
			file_info->size_unit = 'G';
		}
		else if (file_info->size >= 1048576) {
			file_info->readable_size = (ls_double_t)(file_info->size) / 1048576;
			file_info->size_unit = 'M';
		}
		else if (file_info->size >= 1024) {
			file_info->readable_size = (ls_double_t)(file_info->size) / 1024;
			file_info->size_unit = 'K';		
		}
		else {
			file_info->readable_size = file_info->size;
			file_info->size_unit = '\0';
		}
	}
}

static char *
ls_print_get_path_name(char *file_path, const char *dir_path, ls_mode_t mode,
			ls_options_t *options, ls_pool_t *pool)
{
	char	*path_name, *path;
	char	indicator;

	path_name = file_path;

	if (ls_options_is_long_format(options)) {
		indicator = ls_print_get_append_indicator(mode);
			
		if (!S_ISLNK(mode)) {
			if (ls_options_is_append_indicator(options) && indicator != '\0') {
				path_name = ls_print_path_combine(path_name, indicator, pool);
			}				
		}
		else {
			path = ls_print_path_combine4(dir_path, file_path, pool);
					
			ls_ssize_t	name_size = readlink(path, 
						max_path_buf, ls_file_path_max_length());

			if (name_size != -1) {
				max_path_buf[name_size] = '\0';
				struct stat		stat_buf1;
				if (stat(path, &stat_buf1) < 0) {
					path_name = ls_print_path_combine2(path_name,
						" -> ", max_path_buf, pool);
				}
				else {
					indicator = ls_print_get_append_indicator(stat_buf1.st_mode);							
					if (ls_options_is_append_indicator(options) && indicator != '\0') {
						path_name = ls_print_path_combine3(path_name,
							" -> ", max_path_buf, indicator, pool);
					}
					else {
						path_name = ls_print_path_combine2(path_name,
							" -> ", max_path_buf, pool);
					}
				}
			}
		}
	}
	else {
		indicator = ls_print_get_append_indicator(mode);
		if (ls_options_is_append_indicator(options) && indicator != '\0') {
			path_name = ls_print_path_combine(path_name, indicator, pool);
		}				
	
	}

	return path_name;
}

static char
ls_print_get_append_indicator(ls_mode_t	mode)
{

	/* directory */
	if (S_ISDIR(mode)) {
		return indicator[0];
	}
	/* executable */
	else if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH)) {
		return indicator[1];
	}
	/* symbolic link */
	else if (S_ISLNK(mode)) {
		return indicator[2];
	}
#ifdef S_ISWHT
	/* whiteout */
	else if (S_ISWHT(mode)) {
		return indicator[3];
	}
#endif
	/* socket */
	else if (S_ISSOCK(mode)) {
		return indicator[4];
	}
	/* FIFO */
	else if (S_ISFIFO(mode)) {
		return indicator[5];
	}

	return '\0';
}

static char *
ls_print_path_combine(char *path1, char c, ls_pool_t *pool)
{
	ls_size_t	size1, size2;
	char		*path;

	size1 = ls_strlen(path1);
	size2 = sizeof(char);

	path = (char *)ls_pool_alloc(pool, sizeof(char) * size1 + size2 + 1);
	ls_memcpy(path, path1, size1);
	path[size1] = c;
	path[size1 + size2] = '\0';

	return path;
}


static char *
ls_print_path_combine2(char *path1, char *path2, char *path3, ls_pool_t *pool)
{
	ls_size_t	size1, size2, size3;
	char		*path;

	size1 = ls_strlen(path1);
	size2 = ls_strlen(path2);
	size3 = ls_strlen(path3);

	path = (char *)ls_pool_alloc(pool, sizeof(char) * size1 + size2 + size3 + 1);
	ls_memcpy(path, path1, size1);
	ls_memcpy(path + size1, path2, size2);
	ls_memcpy(path + size1 + size2, path3, size3);
	path[size1 + size2 + size3] = '\0';

	return path;
}

static char *
ls_print_path_combine3(char *path1, char *path2, char *path3, char c, ls_pool_t *pool)
{
	ls_size_t	size1, size2, size3, size4;
	char		*path;

	size1 = ls_strlen(path1);
	size2 = ls_strlen(path2);
	size3 = ls_strlen(path3);
	size4 = sizeof(char);

	path = (char *)ls_pool_alloc(pool, sizeof(char) * size1 + size2 + size3 + size4 + 1);
	ls_memcpy(path, path1, size1);
	ls_memcpy(path + size1, path2, size2);
	ls_memcpy(path + size1 + size2, path3, size3);
	path[size1 + size2 + size3] = c;
	path[size1 + size2 + size3 + size4] = '\0';

	return path;
}

static char *
ls_print_path_combine4(const char *dir, const char *name, ls_pool_t *pool)
{
	ls_size_t	dir_size, name_size, size;
	char		*path;

	dir_size = ls_strlen(dir);
	name_size = ls_strlen(name);
	size = dir_size + name_size + 1;
	if (dir[dir_size - 1] != '/') {
		++size;
	}

	path = ls_pool_alloc(pool, size);
	ls_memcpy(path, dir, dir_size);
	if (dir[dir_size - 1] != '/') {
		path[dir_size++] = '/';
	}
	ls_memcpy(path + dir_size, name, name_size);
	return path;
}
