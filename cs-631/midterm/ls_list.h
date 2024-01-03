/*
 * ls_list.h
 * Copyright (C) Lingfei Hu
 */

#ifndef _LS_LIST_H_INCLUDED_
#define _LS_LIST_H_INCLUDED_

#include <ls_core.h>

#define ls_list_empty(list)		((list)->head->next == LS_NULL)

typedef struct ls_list_node_s	ls_list_node_t;

struct ls_list_node_s {
	void				*value;
	ls_list_node_t		*next;
};

typedef struct {
	ls_list_node_t		*head;
	ls_list_node_t		*tail;
} ls_list_t;

ls_list_t *ls_list_create(void);
void ls_list_push_front(ls_list_t *list, void *value);
void ls_list_push_back(ls_list_t *list, void *value);
void ls_list_pop_front(ls_list_t *list);
void ls_list_destory(ls_list_t *list);

#endif /* _LS_LIST_H_INCLUDED_ */
