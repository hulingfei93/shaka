/*
 * ls_process.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_PROCESS_H_INCLUDED_
#define _LS_PROCESS_H_INCLUDED_

#include <ls_core.h>

void ls_process_files(ls_vector_t *file_paths, ls_options_t *options);
void ls_process_dirs(ls_vector_t *dir_paths, ls_options_t *options);

#endif /* _LS_PROCESS_H_INCLUDED_ */
