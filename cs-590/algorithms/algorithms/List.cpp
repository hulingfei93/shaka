#include <iostream>
#include "List.h"

list::list()
{
    p_head = new list_node();
    p_head->p_next = 0;
}

void list::insert(int value)
{
    list_node *new_node = new list_node();
    new_node->value = value;
    new_node->p_next = p_head->p_next;
    p_head->p_next = new_node;
}

void list::print()
{
    list_node *c = p_head->p_next;
    while (c != 0)
    {
        std::cout << c->value << "\t";
        c = c->p_next;
    }
    std::cout << std::endl;
}

void list::reverse()
{
    list_node *prev = 0;
    list_node *cur = p_head->p_next;
    list_node *next = 0;

    while (cur != 0)
    {
        next = cur->p_next;
        cur->p_next = prev;
        prev = cur;
        cur = next;
    }

    p_head->p_next = prev;
}

void list::reverse_resursive()
{
    reverse_resursive_imp(0, p_head->p_next, 0);
}

void list::reverse_resursive_imp(list_node *prev, list_node *cur, list_node *next)
{
    if (cur == 0)
        p_head->p_next = prev;
    else
    {
        next = cur->p_next;
        cur->p_next = prev;
        prev = cur;
        cur = next;
        reverse_resursive_imp(prev, cur, next);
    }
}