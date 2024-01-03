/*
 * ls_log.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_LOG_H_INCLUDED_
#define _LS_LOG_H_INCLUDED_

#include <ls_core.h>

void ls_log_err_exit(const char *fmt, ...);
void ls_log_err(const char *fmt, ...);

#endif /* _LS_LOG_H_INCLUDED_ */
