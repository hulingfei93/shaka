/*
 * ls_list.c
 * Copyright (C) Lingfei Hu
 */

#include <ls_core.h>

ls_list_t *
ls_list_create(void)
{
	ls_list_t		*list;
	ls_list_node_t	*head_node;

	list = (ls_list_t *)ls_alloc(sizeof(ls_list_t));
	list->head = LS_NULL;
	list->tail = LS_NULL;

	head_node = (ls_list_node_t *)ls_alloc(sizeof(ls_list_node_t));
	head_node->value = LS_NULL;
	head_node->next = LS_NULL;

	list->head = head_node;

	return list;
}

void
ls_list_push_front(ls_list_t *list, void *value)
{
	ls_list_node_t	*new_node;
	
	new_node = (ls_list_node_t *)ls_alloc(sizeof(ls_list_node_t));
	new_node->value = value;
	new_node->next = list->head->next;
	list->head->next = new_node;

	if (new_node->next == LS_NULL) {
		list->tail = new_node;
	}
}

void
ls_list_push_back(ls_list_t *list, void *value)
{
	if (ls_list_empty(list)) {
		ls_list_push_front(list, value);
	}
	else {
		ls_list_node_t	*new_node;
	
		new_node = (ls_list_node_t *)ls_alloc(sizeof(ls_list_node_t));
		new_node->value = value;
		new_node->next = LS_NULL;
		list->tail->next = new_node;
		list->tail = new_node;
	}
}

void
ls_list_pop_front(ls_list_t *list)
{
	if (ls_list_empty(list)) {
		return;
	}

	ls_list_node_t	*first_node;

	first_node = list->head->next;
	list->head->next = first_node->next;
	if (ls_list_empty(list)) {
		list->tail = LS_NULL;
	}
	ls_free(first_node);
}

void
ls_list_destory(ls_list_t *list)
{
	ls_list_node_t	*first_node;

	while (!ls_list_empty(list)) {
		first_node = list->head->next;
		list->head->next = first_node->next;
		ls_free(first_node);
	}

	ls_free(list);
}

