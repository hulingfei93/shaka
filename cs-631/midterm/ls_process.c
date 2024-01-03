/*
 * ls_process.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

static void ls_process_file(ls_vector_t *file_info_vector, char *file_path, 
            ls_options_t *options, ls_pool_t *pool);
static char * ls_process_path_combine(char *dir, char *name);
static int ls_cmp_fn_time(const void *p1, const void *p2);
static int ls_cmp_fn_size(const void *p1, const void *p2);
static int ls_cmp_fn_pathname(const void *p1, const void *p2);
static void ls_process_sort(ls_vector_t *file_info_vector, ls_options_t *options);
static void ls_process_dirs_imp(ls_vector_t *file_info_stack, ls_options_t *options);
static void ls_process_file2(ls_vector_t *file_info_vector, ls_fd_t dir_fd, char *name, ls_options_t *options, ls_pool_t *pool);
static void ls_process_fetch_file_info(ls_file_info_t *file_info, struct stat *stat_buf, char *file_path, ls_options_t *options, ls_pool_t *pool);


static void
ls_process_sort(ls_vector_t *file_info_vector, ls_options_t *options)
{
    int (*cmp_fn)(const void *, const void*);

    cmp_fn = LS_NULL;
    if (ls_options_is_long_format(options)) {
        if (ls_options_is_sort_by_time(options)) {
            cmp_fn = ls_cmp_fn_time;
        }
        else if (ls_options_is_sort_by_size(options)) {
            cmp_fn = ls_cmp_fn_size;
        }
        else if (ls_options_is_no_sort(options)) {
        }
        else {  /* sort by path name */
            cmp_fn = ls_cmp_fn_pathname;
        }
    }
    else {
        if (ls_options_is_sort_by_time(options)
            || ls_options_is_print_ctime(options)
            || ls_options_is_print_atime(options)) {
            cmp_fn = ls_cmp_fn_time;
        }
        else if (ls_options_is_sort_by_size(options)) {
            cmp_fn = ls_cmp_fn_size;;
        }
        else if (ls_options_is_no_sort(options)) {
        }
        else {  /* sort by path name */
            cmp_fn = ls_cmp_fn_pathname;
        }
    }
    
    if (cmp_fn != LS_NULL) {
        qsort(file_info_vector->data, file_info_vector->size,
                file_info_vector->elem_size, cmp_fn);
    }

    if (ls_options_is_reverse_sort(options)) {
        /* reverse vector */
        ls_vector_reverse(file_info_vector);
    }
}

static int
ls_cmp_fn_time(const void *p1, const void *p2)
{
    ls_file_info_t *f1, *f2;

    f1 = *(ls_file_info_t **) p1;
    f2 = *(ls_file_info_t **) p2;

    if (f1->time.tv_sec > f2->time.tv_sec) {
        return -1;
    }
    else if (f1->time.tv_sec < f2->time.tv_sec) {
        return 1;
    }
    else {
        if (f1->time.tv_nsec > f2->time.tv_nsec) {
            return -1;
        }
        else if (f1->time.tv_nsec < f2->time.tv_nsec) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

static int
ls_cmp_fn_size(const void *p1, const void *p2)
{
    ls_file_info_t *f1, *f2;

    f1 = *(ls_file_info_t **) p1;
    f2 = *(ls_file_info_t **) p2;
    
    if (f1->size > f2->size) {
        return -1;
    }
    else if (f1->size < f2->size) {
        return 1;
    }

    return 0;

}

static int
ls_cmp_fn_pathname(const void *p1, const void *p2)
{
    return ls_strcmp((*(ls_file_info_t **)p1)->path_name, 
                (*(ls_file_info_t **)p2)->path_name);
}

void
ls_process_dirs(ls_vector_t *dir_paths, ls_options_t *options)
{
    ls_pool_t       *pool;
    ls_int_t        i;
    char            **path_ptr;
    ls_vector_t     *file_info_vector;
    ls_vector_t     *dir_stack;
    ls_file_info_t  *dir_info;
    char            *dir_path;
    size_t          dir_path_size;

    pool = ls_pool_create(LS_POOL_NORMAL_SIZE);

    file_info_vector = ls_vector_create(
        sizeof(ls_file_info_t *), dir_paths->size, pool);

    /* get the info of each dir */
    for (i = 0; i < (ls_int_t)(dir_paths->size); ++i) {
        path_ptr = (char **)ls_vector_get(dir_paths, i);
        ls_process_file(file_info_vector, *path_ptr, options, pool);
    }

    /* sort */
    ls_process_sort(file_info_vector, options);
    
    if (ls_options_is_dir_no_search(options)) {
        ls_print(file_info_vector, LS_NULL, options, pool);
    }
    else {
        dir_stack = ls_vector_create(sizeof(char *), LS_VECTOR_NORMAL_SIZE, pool);
        for (i = (ls_int_t)(file_info_vector->size) - 1; i >= 0; --i) {
            dir_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);
            
            dir_path_size = ls_strlen(dir_info->path_name) + 1;
            dir_path = ls_alloc(dir_path_size);
            ls_memcpy(dir_path, dir_info->path_name, dir_path_size);

            ls_vector_push_back(dir_stack, &dir_path);

        }
        ls_process_dirs_imp(dir_stack, options);
    }
    
    ls_pool_destory(pool);
}

static void
ls_process_dirs_imp(ls_vector_t *file_info_stack, ls_options_t *options)
{
    ls_pool_t       *pool;  /* pool used for each dir traverse */
    DIR             *dir;
    struct dirent   *dirp;
    ls_file_info_t  *file_info;
    ls_fd_t         cur_dir_fd;
    ls_vector_t     *file_info_vector;
    ls_int_t        i;
    char            *dir_path, *dir_path_copy, *new_dir_path;
    ls_size_t       dir_path_size;


    while (!ls_vector_is_empty(file_info_stack)) {
        pool = ls_pool_create(LS_POOL_NORMAL_SIZE);

        /* get the top dir from stack and remove top */
        dir_path = *(char **)ls_vector_top(file_info_stack);
        ls_vector_pop(file_info_stack);
        dir_path_size = ls_strlen(dir_path);
        dir_path_copy = (char *)ls_pool_alloc(pool, dir_path_size + 1);
        ls_memcpy(dir_path_copy, dir_path, dir_path_size + 1);
        ls_free(dir_path);

    
        file_info_vector = ls_vector_create(
            sizeof(ls_file_info_t *), LS_VECTOR_NORMAL_SIZE , pool);
        
        if ((dir = opendir(dir_path_copy)) == LS_NULL) {
            ls_log_err("ls: opendir error %s: %s\n", dir_path_copy, strerror(errno));
            continue;
        }
        if ((cur_dir_fd = open(dir_path_copy, O_RDONLY)) == -1) {
            ls_log_err("ls: open error %s: %s\n", dir_path_copy, strerror(errno));
            continue;
        }

        /* traverse the content of dir */
        while ((dirp = readdir(dir)) != LS_NULL) {
            if ((ls_strcmp(dirp->d_name, ".") == 0 || ls_strcmp(dirp->d_name, "..") == 0)
                        && !ls_options_is_print_cur_parent(options)) {
                continue;
            }
            if (dirp->d_name[0] == '.' && !ls_options_is_print_hidden(options)) {
                continue;
            }

            /* fetch info for each file in dir */
            ls_process_file2(file_info_vector, cur_dir_fd, dirp->d_name, options, pool);

        }
        close(cur_dir_fd);
        closedir(dir);
    
        /* sort */
        ls_process_sort(file_info_vector, options);

        /* traverse dirs recursively */
        if (ls_options_is_subdir_recursive(options)) {
            for (i = (ls_int_t)(file_info_vector->size) - 1; i >= 0; --i) {
                file_info = *(ls_file_info_t **)ls_vector_get(file_info_vector, i);
                if (!S_ISDIR(file_info->mode)) {
                    continue;
                }
                if (ls_strcmp(file_info->path_name, ".") == 0 
                                || ls_strcmp(file_info->path_name, "..") == 0) {
                    continue;
                }
                if (file_info->path_name[0] == '.' 
                            && !ls_options_is_print_hidden(options)) {
                    continue;
                }
                new_dir_path = ls_process_path_combine(dir_path_copy, 
                            file_info->path_name);
                
                ls_vector_push_back(file_info_stack, &new_dir_path);
            }
        }
        
        /* print */
        ls_print(file_info_vector, dir_path_copy, options, pool);
        
        /* clean up */
        ls_pool_destory(pool);
    } // end while

}

void
ls_process_files(ls_vector_t *file_paths, ls_options_t *options)
{
    ls_pool_t       *pool;
    ls_uint_t       i;
    char            **path_ptr;
    ls_vector_t     *file_info_vector;

    pool = ls_pool_create(LS_POOL_NORMAL_SIZE);

    file_info_vector = ls_vector_create(
        sizeof(ls_file_info_t *), file_paths->size, pool);

    /* get the info of each file */
    for (i = 0; i < file_paths->size; ++i) {
        path_ptr = (char **)ls_vector_get(file_paths, i);
        ls_process_file(file_info_vector, *path_ptr, options, pool);
    }

    /* sort */
    ls_process_sort(file_info_vector, options);

    /* print */
    ls_print(file_info_vector, LS_NULL, options, pool);
    
    ls_pool_destory(pool);
}

static void
ls_process_file(ls_vector_t *file_info_vector, char *file_path,
            ls_options_t *options, ls_pool_t *pool)
{
    ls_file_info_t          *file_info;
    struct stat             stat_buf;

    if (lstat(file_path, &stat_buf) < 0) {
        ls_log_err("ls: cannot access %s: %s\n", file_path, strerror(errno));
        return;
    }

    file_info = (ls_file_info_t *)ls_pool_alloc(pool, sizeof(ls_file_info_t));

    ls_process_fetch_file_info(file_info, &stat_buf, file_path, options, pool);

    ls_vector_push_back(file_info_vector, &file_info);
}

static void
ls_process_file2(ls_vector_t *file_info_vector, ls_fd_t dir_fd,
            char *name, ls_options_t *options, ls_pool_t *pool)
{
    ls_file_info_t          *file_info;
    struct stat             stat_buf;

    if (fstatat(dir_fd, name, &stat_buf, AT_SYMLINK_NOFOLLOW) < 0) {
        ls_log_err("ls: cannot access %s: %s\n", name, strerror(errno));
        return;
    }

    file_info = (ls_file_info_t *)ls_pool_alloc(pool, sizeof(ls_file_info_t));

    ls_process_fetch_file_info(file_info, &stat_buf, name, options, pool);

    ls_vector_push_back(file_info_vector, &file_info);
}

static void
ls_process_fetch_file_info(ls_file_info_t *file_info, 
        struct stat *stat_buf, char *file_path, 
        ls_options_t *options, ls_pool_t *pool)
{
    ls_size_t   path_name_size;
    /* path name */
    path_name_size = ls_strlen(file_path);
    file_info->path_name = (char *)ls_pool_alloc(pool, path_name_size + 1);
    ls_memcpy(file_info->path_name, file_path, path_name_size + 1);
    
    /* inode number */
    file_info->inode_number = stat_buf->st_ino;

    /* block number */
    file_info->block_number = stat_buf->st_blocks;

    /* file type & mode */
    file_info->mode = stat_buf->st_mode;

    /* link numbers */
    file_info->link_number = stat_buf->st_nlink;

    /* owner id */
    file_info->owner_id = stat_buf->st_uid;

    /* group id */
    file_info->group_id = stat_buf->st_gid;

    /* size in bytes */
    file_info->size = stat_buf->st_size;

    /* time */
    if (ls_options_is_print_ctime(options)) {
            file_info->time = stat_buf->st_ctim;
    }
    else if (ls_options_is_print_atime(options)) {
        file_info->time = stat_buf->st_atim;
    }
    else {
        file_info->time = stat_buf->st_mtim;
    }
}

static char *
ls_process_path_combine(char *dir, char *name)
{
    ls_size_t   dir_size, name_size, size;
    char        *path;

    dir_size = ls_strlen(dir);
    name_size = ls_strlen(name);
    size = dir_size + name_size + 1;
    if (dir[dir_size - 1] != '/') {
        ++size;
    }

    path = ls_calloc(size);
    ls_memcpy(path, dir, dir_size);
    if (dir[dir_size - 1] != '/') {
        path[dir_size++] = '/';
    }
    ls_memcpy(path + dir_size, name, name_size);
    return path;
}
