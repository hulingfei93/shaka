/*
 * sish_log.h
 */

#ifndef _SISH_LOG_H_INCLUDED_
#define _SISH_LOG_H_INCLUDED_

#include <sish_core.h>

extern sish_int_t   last_exit_code;

void sish_log_err_exit(const char *fmt, ...);
void sish_log_err(const char *fmt, ...);

#endif /* _SISH_LOG_H_INCLUDED_ */
