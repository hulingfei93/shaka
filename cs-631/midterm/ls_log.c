/*
 * ls_log.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

void
ls_log_err_exit(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    exit(LS_EXIT_FAILURE);
}

void
ls_log_err(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
