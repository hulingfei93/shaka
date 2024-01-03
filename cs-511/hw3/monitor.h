/*
* monitor.h
* Copyright (C) Lingfei Hu
*/

#ifndef _MONITOR_H_INCLUDED_
#define _MONITOR_H_INCLUDED_

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define true    1
#define false   0

#define Q_NORTH 'n'
#define Q_SOUTH 's'
#define Q_EAST 'e'
#define Q_WEST 'w'

typedef unsigned char bool;

struct cart_t {
  int num;       /* number of this cart */
  char dir;      /* direction from which this cart arrives at intersection */
};

struct q_entry_t {
  struct cart_t *cart;
  struct q_entry_t *next;
};

void err_exit(const char *, ...);
struct cart_t *q_getCart(char);
void monitor_init(char *);
void monitor_shutdown(void);
void monitor_arrive(struct cart_t *);
void monitor_cross(struct cart_t *);
void monitor_leave(struct cart_t *);

#endif /* _MONITOR_H_INCLUDED_ */

