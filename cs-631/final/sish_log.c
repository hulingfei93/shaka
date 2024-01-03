/*
 * sish_log.c
 */

#include <sish_core.h>

sish_int_t  last_exit_code;

void
sish_log_err_exit(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    exit(SISH_EXIT_FAILURE);
}

void
sish_log_err(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
