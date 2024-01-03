/*
 * ls_print.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_PRINT_H_INCLUDED_
#define _LS_PRINT_H_INCLUDED_

#include <ls_core.h>

#define LS_FORMAT_BUF_SIZE      256
#define LS_DATA_BUF_SIZE        1024

typedef struct {
    ls_ino_t    max_inode;
    ls_size_t   max_inode_size;

    ls_blkcnt_t max_block_number;
    ls_size_t   max_block_number_size;
    
    ls_blkcnt_t max_converted_block_number;
    ls_size_t   max_converted_block_number_size;
    
    ls_size_t   max_mode_size;

    ls_nlink_t  max_link_number;
    ls_size_t   max_link_number_size;
    
    ls_uid_t    max_owner_id;
    ls_size_t   max_owner_id_size;
    ls_size_t   max_owner_size;

    ls_gid_t    max_group_id;
    ls_size_t   max_group_id_size;
    ls_size_t   max_group_size;

    ls_off_t    max_size;
    ls_size_t   max_size_size;

    ls_double_t max_readable_size;
    ls_size_t   max_readable_size_size;
    
    ls_size_t   max_month_size;

    ls_uint_t   max_day;
    ls_size_t   max_day_size;
    
    ls_size_t   max_hour_min_size;

    ls_size_t   max_pathname_size;
} ls_print_info_t;

void ls_print(ls_vector_t *file_info_vector, const char *dir_path, ls_options_t *options, ls_pool_t *pool);
void ls_print_init(ls_pool_t *pool);

#endif /* _LS_PRINT_H_INCLUDED_ */
