/*
 * ls_file.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_FILE_H_INCLUDED_
#define _LS_FILE_H_INCLUDED_

#include <ls_core.h>

#define SSE_FD_STDIN        STDIN_FILENO    
#define SSE_FD_STDOUT       STDOUT_FILENO
#define SSE_FD_STDERR       STDERR_FILENO

#define LS_FILE_MODE_STRING_LENGTH  12

#define ls_options_is_dir_no_search(options) \
    ((options)->flag_d)

#define ls_options_is_long_format(options) \
    ((options)->flag_l || (options)->flag_n)

#define ls_options_is_append_indicator(options) \
    ((options)->flag_F)

#define ls_options_is_print_inode_number(options) \
    ((options)->flag_i)

#define ls_options_is_print_block_number(options) \
    ((options)->flag_s)

#define ls_options_is_print_ctime(options) \
    ((options)->flag_c)

#define ls_options_is_print_atime(options) \
    ((options)->flag_u)

#define ls_options_is_dereference_symlink(options) \
    (!(options)->flag_d)

#define ls_options_is_sort_by_time(options) \
    ((options)->flag_t)

#define ls_options_is_sort_by_size(options) \
    ((options)->flag_S)

#define ls_options_is_no_sort(options) \
    ((options)->flag_f)

#define ls_options_is_reverse_sort(options) \
    ((options)->flag_r)

#define ls_options_is_print_hidden(options) \
    ((options)->flag_A || (options)->flag_a)

#define ls_options_is_print_cur_parent(options) \
    ((options)->flag_a)

#define ls_options_is_subdir_recursive(options) \
    ((options)->flag_R)

#define ls_options_is_print_size_readable(options) \
    ((options)->flag_h)

#define ls_options_is_print_size_kb(options) \
    ((options)->flag_k)

#define ls_options_is_print_replace_nonprintable(options) \
    ((options)->flag_q || ((options)->flag_terminal && !(options)->flag_w))

#define ls_options_is_print_block_number(options) \
    ((options)->flag_s)

#define ls_options_is_print_owner_group_name(options) \
    ((options)->flag_l)

typedef struct {
    /* 
     * filter flags 
     */
    ls_bool_t   flag_A;
    ls_bool_t   flag_a;

    /* 
     * display format & info flags 
     */
    /*
     * -C, -l, -1, -n and -x override each other, the last one 
     *  specified determins the format used.
     */
    ls_bool_t   flag_C;
    ls_bool_t   flag_l;
    ls_bool_t   flag_1;
    ls_bool_t   flag_n;
    ls_bool_t   flag_x;
    
    ls_bool_t   flag_F;


    /* -h -k override each other*/
    ls_bool_t   flag_h;
    ls_bool_t   flag_k;
    
    /* 
     * -w -q override each other, the last one specified
     * determines the format used for non-print-able characters.
     */
    ls_bool_t   flag_q;
    ls_bool_t   flag_w;

    /* 
     * display info flags
     */
    ls_bool_t   flag_i;  /* print inode number */
    ls_bool_t   flag_s;  /* print block number */

    /* 
     * time and sort flags 
     */
    /* 
     * -c and -u override each other, the last one specified 
     * determines the file time used.  
     */
    ls_bool_t   flag_c;  /* display time of last file status change */
    ls_bool_t   flag_u;  /* display time of last access */

    /* 
     * -t, -S and -f override each other, the last one specified
     * determines the sort field 
     */
    ls_bool_t   flag_f;  /* not sorted */
    ls_bool_t   flag_S;  /* sort by size, largest file first */
    ls_bool_t   flag_t;  /* sort by time of last modification */

    ls_bool_t   flag_r;  /* reserse order  */

    /* 
     * recursively list flags 
     */
    ls_bool_t   flag_d;
    ls_bool_t   flag_R;

    /*
     * others
     */
    ls_bool_t   flag_terminal;

} ls_options_t;

typedef struct {
    char            *path_name;
    ls_ino_t        inode_number;
    ls_blkcnt_t     block_number;
    ls_mode_t       mode;
    ls_nlink_t      link_number;
    ls_uid_t        owner_id;
    ls_gid_t        group_id;
    ls_off_t        size;
    ls_timespec_t   time;

    ls_double_t     readable_size;
    ls_blkcnt_t     converted_block_number;
    char            *owner_name;
    char            *group_name;
    char            size_unit;
} ls_file_info_t;

ls_bool_t ls_file_is_dir(const char *path);
ls_size_t ls_file_path_max_length(void);
void ls_options_format_flags_clear(ls_options_t *options);
void ls_options_printable_flags_clear(ls_options_t *options);
void ls_options_time_flags_clear(ls_options_t *options);
void ls_options_sort_flags_clear(ls_options_t *options);
void ls_options_size_display_flags_clear(ls_options_t *options);

#endif /* _LS_FILE_H_INCLUDED_ */
